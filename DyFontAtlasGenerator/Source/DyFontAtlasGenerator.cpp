#include "precompiled.h"
///
/// MIT License
/// Copyright (c) 2018-2019 Jongmin Yun
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.
///

/// Header file
#include <DyFontAtlasGenerator.h>

#include <thread>
#include <optional>

#include <QtOpenGL>
#include <QtConcurrent/QtConcurrentRun>
#include <QThread>

#include <ft2build.h>
#if !defined(FT_FREETYPE_H)
#define FT_FREETYPE_H
#endif
#include <freetype/freetype.h>
#include <freetype/ftoutln.h>
#include <spdlog/fmt/fmt.h>
#include <nlohmann/json.hpp>

#include <Include/Library/MsdfgenHelper.h>
#include <Include/Library/HelperZlib.h>
#include <Include/Library/JsonTypeWriterHelper.h>
#include <Include/Type/CoordinateBounds.h>
#include <Include/QtHelper/PaintSurface.h>
#include <Include/Structure.h>

#include <Include/AConstantLangRange.h>
#include <Include/Library/HelperString.h>
#include <Include/Type/DFileInformations.h>
#include <DyWindowAbout.h>

namespace dy
{

auto sFtLibraryList {std::vector<FT_Library>{}};
auto sFtFaceList    {std::vector<FT_Face>{}};
std::atomic<bool> sFtIsInitiailzed {false};

using TSdfType          = msdfgen::Bitmap<float>;
using TCharMapRangePair = std::pair<uint64_t, uint64_t>;

[[nodiscard]] bool ExportAsSeparateJsonAndPng (const DDyFontInformation& fontMetaInfo, const nlohmann::json& fontJsonInfo, const std::vector<QImage>& fontImage);
[[nodiscard]] bool ExportAsIntegratedFile     (const DDyFontInformation& fontMetaInfo, const nlohmann::json& fontJsonInfo, const std::vector<QImage>& fontImage);

/*  TEMPLATE
 *  00......`.......08......`.......
    [PrevFontPtr   ][NextFontPtr   ]
    16......`.......24......`.......
    [PlainInfoLen  ][CompInfoLength]
    32......`.......48......`.......
    [PlainImgLength][CompImgLength ]
    64..............................
    [Compressed Information Buffer ]
    64+CompInfoLen..................
    [Compressed Image Buffer       ]
    .............................EOF
 */

/// @struct DBound2D
/// @brief
struct DBound2D final
{
  double l = std::numeric_limits<double>::max();
  double b = std::numeric_limits<double>::max();
  double r = std::numeric_limits<double>::lowest();
  double t = std::numeric_limits<double>::lowest();
};

/// @struct DResult
/// @brief
struct DResult final
{
  dy::DDyCoordinateBounds mCoordinateBound;
  QImage                  mImageBuffer;
  nlohmann::json          mItemJsonAtlas;
  FT_ULong                mCharCode{};
};

/// @struct DAlignedBBoxInfo
/// @brief
struct DAlignedBBoxInfo final
{
  double            range{};
  msdfgen::Vector2  scale;
  msdfgen::Vector2  translate;
};

/// @brief
/// @param value
void DyResizeFreetypeList(const int32_t value)
{
  Q_ASSERT(value > 0);
  Q_ASSERT(sFtIsInitiailzed.load() == false);

  sFtLibraryList.resize(value);
  sFtFaceList.resize(value);
}

void DyInitializeFreetype() noexcept
{
  // Check Freetype is we`ll.
  for (auto i {0u}; i < sFtLibraryList.size(); ++i)
  {
    if (FT_Init_FreeType(&sFtLibraryList[i]) != 0) { }
  }

  auto previous {false};
  while (sFtIsInitiailzed.compare_exchange_strong(previous, true, std::memory_order::memory_order_seq_cst) == false)
    ;
}

void DyLoadFontFreetype(const QString fontPath) noexcept
{
  Q_ASSERT(sFtIsInitiailzed.load() == true);
  for (auto i {0u}; i < sFtFaceList.size(); ++i)
  {
    Q_ASSERT(sFtLibraryList[i] != nullptr);
    if (FT_New_Face(sFtLibraryList[i], fontPath.toStdString().c_str(), 0, &sFtFaceList[i])) { }
  }
}

void DyReleaseFreetype() noexcept
{
  Q_ASSERT(sFtIsInitiailzed.load() == true);

  for (auto i {0u}; i < sFtFaceList.size(); ++i)
  {
    Q_ASSERT(sFtLibraryList[i]  != nullptr);
    Q_ASSERT(sFtFaceList[i]     != nullptr);

    if (FT_Done_Face(sFtFaceList[i]) != 0) { }
    if (FT_Done_FreeType(sFtLibraryList[i]) != 0) { }

    sFtFaceList[i]    = nullptr;
    sFtLibraryList[i] = nullptr;
  }

  auto previous {true};
  while (sFtIsInitiailzed.compare_exchange_strong(previous, false, std::memory_order::memory_order_seq_cst) == false)
    ;
}

///
/// @brief
/// @param  buffer
/// @return RVOed QImage (copyable)
///
[[nodiscard]] QImage CreateQImageFromSDFBuffer(const TSdfType& buffer)
{
  auto imageBuffer {QImage{dy::TEXTURE_SIZE_S, TEXTURE_SIZE_T, QImage::Format::Format_RGB32}};

  for (auto y{ 0 }; y < TEXTURE_SIZE_T; ++y)
  {
    for (auto x{ 0 }; x < TEXTURE_SIZE_S; ++x)
    {
      QRgb value;
      const auto changedUCharValue{ static_cast<uint8_t>(std::floor(std::max(buffer(x, y), 0.f) * 255)) };
      // Just one.
      value = qRgb(changedUCharValue, 0, 0);
      imageBuffer.setPixel(x, y, value);
    }
  }

  return imageBuffer;
};

/// @brief Checks Bounds boundary veridity.
void CheckAndChangeBounds(DBound2D& bounds)
{
  if (bounds.l >= bounds.r || bounds.b >= bounds.t)
  {
    bounds.l = 0; bounds.b = 0; bounds.r = 1; bounds.t = 1;
  }
};

/// @brief
[[nodiscard]] DAlignedBBoxInfo AlignSDFBBoxFrame(const DBound2D& bounds)
{
  const auto dims{ msdfgen::Vector2{bounds.r - bounds.l, bounds.t - bounds.b} };
  auto translate{ msdfgen::Vector2{} };
  auto scale{ msdfgen::Vector2{1, 1} };
  auto frame{ msdfgen::Vector2{
      static_cast<double>(TEXTURE_SIZE_S),
      static_cast<double>(TEXTURE_SIZE_T)
  } };
  frame -= 2 * TEXTURE_PXRANGE;

  if (dims.x * frame.y < dims.y * frame.x)
  {
    translate.set(.5 * (frame.x / frame.y * dims.y - dims.x) - bounds.l, -bounds.b);
    scale = frame.y / dims.y;
  }
  else
  {
    translate.set(-bounds.l, .5 * (frame.y / frame.x * dims.x - dims.y) - bounds.b);
    scale = frame.x / dims.x;
  }

  translate += TEXTURE_PXRANGE / scale;
  const auto range{ double{TEXTURE_PXRANGE / msdfgen::min(scale.x, scale.y)} };
  return DAlignedBBoxInfo{range, scale, translate };
};

[[nodiscard]] DDyFontInformation GetFontGeneralInformation(const QString fontPath)
{
  FT_Library sFtLibrary = nullptr;
  FT_Face    sFtFace    = nullptr;

  if (FT_Init_FreeType(&sFtLibrary) != 0) { }
  if (FT_New_Face(sFtLibrary, fontPath.toStdString().c_str(), 0, &sFtFace)) { }

  DDyFontInformation result = {};
  result.fontName   = sFtFace->family_name;
  result.fontStyle  = sFtFace->style_name;
  result.fontPath   = fontPath.toStdString();

  if (FT_Done_Face(sFtFace) != 0)         { }
  if (FT_Done_FreeType(sFtLibrary) != 0)  { }
  return result;
}

[[nodiscard]] std::optional<DDyTextFileInformation> GetTextGlyphs(const QString iFontPath)
{
  // Open file.
  const std::string fontPath = iFontPath.toStdString();
  FILE* fdFile = fopen(fontPath.c_str(), "r");
  if (fdFile == nullptr)
  {
    return std::nullopt;
  }

  // Make staging buffer.
  std::fseek(fdFile, 0, SEEK_END);
  const auto fileByteSize = std::ftell(fdFile);
  std::fseek(fdFile, 0, SEEK_SET);

  // Read file until EOF.
  std::vector<char> buffer(fileByteSize);
  std::fread(buffer.data(), sizeof(char), fileByteSize, fdFile);
  assert(std::feof(fdFile));

  // Check.
  std::set<uint16_t> returnedCharGlyphs;
  for (auto it = buffer.data(); *it != '\0'; it += DyGetByteOfUtf8Char(it))
  {
    const auto chr = DyGetRawUtf16CharacterFrom(it);
    if (chr != SURROGATE_SIGN) { returnedCharGlyphs.emplace(chr); }
  }

  // Close file descriptor.
  fclose(fdFile);
  return DDyTextFileInformation{returnedCharGlyphs};
}

void Process(std::vector<DResult>& charResultList, const std::vector<FT_ULong>& charMapList, const TCharMapRangePair charRangePair, const FT_Face ftFace)
{
  const auto itStart  = charMapList.cbegin() + charRangePair.first;
  const auto itEnd    = charMapList.cbegin() + charRangePair.second;

  FT_Outline_Funcs ftFunctions = {};
  ftFunctions.move_to = &dy::ftMoveTo;    ftFunctions.line_to = &dy::ftLineTo;
  ftFunctions.conic_to = &dy::ftConicTo;  ftFunctions.cubic_to = &dy::ftCubicTo;
  ftFunctions.shift = 0;                  ftFunctions.delta = 0;

  for (auto charCodeIt = itStart; charCodeIt != itEnd; ++charCodeIt)
  {
    const FT_Error loadCharErrorFlag = FT_Load_Char(ftFace, *charCodeIt, FT_LOAD_NO_SCALE);
    Q_ASSERT(loadCharErrorFlag == 0);

    msdfgen::Shape          shape = {};
    dy::DDyFreeTypeContext  context = {}; context.mShape = &shape;

    const auto outlineErrorFlag = FT_Outline_Decompose(&ftFace->glyph->outline, &ftFunctions, &context);
    Q_ASSERT(outlineErrorFlag == 0);

    // Set bounds
    DBound2D bounds{};
    shape.normalize(); shape.bounds(bounds.l, bounds.b, bounds.r, bounds.t); shape.inverseYAxis = true;
    CheckAndChangeBounds(bounds);

    // Set translate (non scaled)
    const auto alignedInfo = AlignSDFBBoxFrame(bounds);

    // Generate information
    msdfgen::edgeColoringSimple(shape, 3.0);
    auto sdfFloatBuffer{ TSdfType{TEXTURE_SIZE_S, TEXTURE_SIZE_T} };
    msdfgen::generateSDF(sdfFloatBuffer, shape, alignedInfo.range, alignedInfo.scale, alignedInfo.translate);

    // Realign and scale translate for saving.
    const auto glyphScale{ static_cast<float>(STANDARD_UNITPEREM) / ftFace->units_per_EM };
    auto translate{ alignedInfo.translate };
    translate -= TEXTURE_PXRANGE / alignedInfo.scale;
    translate *= glyphScale;
    translate += TEXTURE_PXRANGE / alignedInfo.scale * glyphScale;

    const uint64_t id = static_cast<uint64_t>(std::distance(charMapList.cbegin(), charCodeIt));
    const auto texCoord = dy::CreateCoordinateInformation(TEXTURE_CANVAS_S, TEXTURE_CANVAS_T, TEXTURE_SIZE_S, TEXTURE_SIZE_T, id);

    // Make json information and return.
    nlohmann::json jsonWrite;
    jsonWrite["Size"] = dy::DDyVector2{ float(ftFace->glyph->metrics.width), float(ftFace->glyph->metrics.height) } *glyphScale;
    jsonWrite["HoriBearing"] = dy::DDyVector2{ float(ftFace->glyph->metrics.horiBearingX), float(ftFace->glyph->metrics.horiBearingY) } *glyphScale;
    jsonWrite["Scale"] = dy::DDyVector2{ float(alignedInfo.scale.x), float(alignedInfo.scale.y) } / glyphScale;
    jsonWrite["Translate"] = dy::DDyVector2{ float(translate.x), float(translate.y) } *16;
    jsonWrite["TexCoordBox"] = texCoord;
    jsonWrite["HoriAdvance"] = ftFace->glyph->metrics.horiAdvance * glyphScale / 64.0;

    charResultList[id] = DResult{ texCoord, CreateQImageFromSDFBuffer(sdfFloatBuffer), jsonWrite, *charCodeIt};
  }
};

/// @brief Create font buffer and export outside by following option.
void CreateFontBuffer(const DDyFontInformation information,
                      const std::vector<FT_ULong> targetCharMap,
                      const dy::EDyOptionCollections option,
                      DyFontAtlasGenerator& parent)
{
  /// @brief Insert meta information of specified font.
  static auto InsertMetaInformationToJson = [&](nlohmann::json& json)
  {
    auto metaInformation{dy::DMeta{}};
    metaInformation.mFontSpecifierName   = information.fontName;
    metaInformation.mFontStyleSpecifier  = information.fontStyle;
    metaInformation.mHoriLinefeed        = sFtFaceList[0]->height;
    json["Meta"] = metaInformation;
  };

  /// @brief Calculate range for searching of each thread.
  static auto CalculateRangeTo = [](uint64_t charMapSize, uint64_t threadCount, std::vector<TCharMapRangePair>& result)
  {
    result.reserve(threadCount);
    for (uint64_t threadId = 0; threadId < threadCount; ++threadId)
    {
      const auto begin = threadId * charMapSize / threadCount;
      const auto end   = (threadId + 1) * charMapSize / threadCount;
      result.emplace_back(std::make_pair(begin, end));
    }
  };

  //! - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //! FunctionBody
  //! - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // Get concurrent thread number.
  const uint32_t concurrentThreadNumber = std::thread::hardware_concurrency();
  const auto     targetCharMapSize      = static_cast<uint64_t>(targetCharMap.size());

  // Freetype initialization with the number of threads to instantiate.
  DyResizeFreetypeList(concurrentThreadNumber);
  DyInitializeFreetype();
  DyLoadFontFreetype  (information.fontPath.c_str());

  // Make paint surface of off-screen rendering.
  CPaintSurface paintSurface;
  paintSurface.resize(TEXTURE_CANVAS_S, TEXTURE_CANVAS_T);
  paintSurface.InitializeContext();
  paintSurface.ClearSurface();
  // Make json information instance for font glyphs.
  nlohmann::json jsonDescriptor;  InsertMetaInformationToJson(jsonDescriptor);

  std::vector<DResult>            charResultList(targetCharMapSize);
  std::vector<TCharMapRangePair>  charRangeList = {};
  std::vector<std::future<void>>  threadResultList = {}; threadResultList.reserve(concurrentThreadNumber);
  CalculateRangeTo(targetCharMapSize, concurrentThreadNumber, charRangeList);

  // Create information.
  for (auto thread = 0u; thread < concurrentThreadNumber; ++thread)
  { // Mutli-thread processing.
    threadResultList.emplace_back(
        std::async(std::launch::async, Process,
            std::ref(charResultList), std::cref(targetCharMap), charRangeList[thread], sFtFaceList[thread]
        )
    );
  }
  for (auto thread = 0u; thread < concurrentThreadNumber; ++thread) { threadResultList[thread].get(); }

  // Create texture atlas.
  std::vector<QImage> drawnImageList {};
  for (uint64_t id = 0u; id < targetCharMapSize; ++id)
  {
    // Get result from charRangeList.
    const auto& result = charResultList[id];
    jsonDescriptor["Characters"][fmt::format("{0}", result.mCharCode)] = result.mItemJsonAtlas;

    // Make QImage from Bitmap<float> and texture from QImage. (RVO guaranted)
    paintSurface.UpdateBufferInformation(result.mCoordinateBound);
    paintSurface.CreatePreviousBufferStateTexture();
    paintSurface.BindTexturePointer(result.mImageBuffer);
    paintSurface.render();

    parent.IncrementProgress();
    if ((id + 1) % TEXTURE_MAPLIMIT == 0)
    { // Export off-screen texture buffer to png or file information (binary).
      drawnImageList.emplace_back(paintSurface.GetImageFromGLFBO());
      paintSurface.ClearSurface();
    }
  }

  threadResultList.clear();
  drawnImageList.emplace_back(paintSurface.GetImageFromGLFBO());
  paintSurface.ClearSurface();

  if (dy::IsHavingFlags(option, dy::EDyOptionCollections::SeparateJsonAndPng) == true)
  {
    const auto isSuccessful = ExportAsSeparateJsonAndPng(information, jsonDescriptor, drawnImageList);
    Q_ASSERT(isSuccessful == true);
  }
  else
  {
    const auto isSuccessful = ExportAsIntegratedFile(information, jsonDescriptor, drawnImageList);
    Q_ASSERT(isSuccessful == true);
  }
  DyReleaseFreetype();
}

[[nodiscard]] bool ExportAsSeparateJsonAndPng(const DDyFontInformation& fontMetaInfo, const nlohmann::json& fontJsonInfo, const std::vector<QImage>& fontImage)
{
  const QString filename = fmt::format("./{}_{}.dyFntRes", fontMetaInfo.fontName, fontMetaInfo.fontStyle).c_str();
  if (QFile file(filename); file.open(QIODevice::WriteOnly))
  {
    const auto compressedJsonResult = dy::zlib::CompressBuffer(fontJsonInfo);
    QDataStream stream(&file);
    stream.writeBytes(compressedJsonResult.mCompressedBuffer.data(), compressedJsonResult.mCompressedLength);
    file.close();
  }

  const auto imageListCount = fontImage.size();
  for (auto imageId = 0u; imageId < imageListCount; ++imageId)
  {
    const auto isSuccessful = fontImage[imageId].save(fmt::format("./{}_{}_{}.png", fontMetaInfo.fontName, fontMetaInfo.fontStyle, imageId).c_str(), "PNG");
    Q_ASSERT(isSuccessful == true);
  }

  return true;
}

[[nodiscard]] bool ExportAsIntegratedFile(const DDyFontInformation& fontMetaInfo, const nlohmann::json& fontJsonInfo, const std::vector<QImage>& fontImage)
{
  static auto ConvertToQByteArrayBuffers = [](const std::vector<QImage>& fontImageList)
  {
    const auto drawnImageListSize = static_cast<int>(fontImageList.size());
    std::vector<QByteArray> atlasBufferList(drawnImageListSize);
    for (auto imageId = 0; imageId < drawnImageListSize; ++imageId)
    {
      const auto& image = fontImageList[imageId];
      const auto width  = image.width();
      const auto height = image.height();
      for (auto pxY = 0; pxY < height; ++pxY)
      {
        for (auto pxX = 0; pxX < width; ++pxX)
        { // Get color and emplace to buffer.
          const auto pixelColor = image.pixelColor(pxX, pxY);
          const auto alpha  = pixelColor.alpha(); const auto red    = pixelColor.red();
          const auto green  = pixelColor.green(); const auto blue   = pixelColor.blue();
          // Insert RGBA32 (8bit each channel) to list.
          atlasBufferList[imageId].push_back(static_cast<char>(red));
          atlasBufferList[imageId].push_back(static_cast<char>(green));
          atlasBufferList[imageId].push_back(static_cast<char>(blue));
          atlasBufferList[imageId].push_back(static_cast<char>(alpha));
        }
      }
    }

    return atlasBufferList;
  };

  static auto CompressImageBufferList = [](const std::vector<QByteArray>& imageBufferList)
  {
    std::vector<dy::zlib::DZlibResult> resultList(imageBufferList.size());
    for (auto imageId = 0u; imageId < imageBufferList.size(); ++imageId)
    {
      resultList[imageId] = dy::zlib::CompressBuffer(imageBufferList[imageId]);
    }
    return resultList;
  };

  const auto compressedJsonResult       = dy::zlib::CompressBuffer(fontJsonInfo);
  const auto compressedImageBufferList  = CompressImageBufferList(ConvertToQByteArrayBuffers(fontImage));

  DHeaderStructure header{};
  header.mPtrPreviousFontAddress  = 0;
  header.mPtrNextFontAddress      = 0;
  header.mInformationBufferLength.mPlainBufferLength      = compressedJsonResult.mDecompressedLength;
  header.mInformationBufferLength.mCompressedBufferLength = compressedJsonResult.mCompressedLength;
  for (const auto& imageResult : compressedImageBufferList)
  {
    DHeaderStructure::DBufferLength length{};
    length.mPlainBufferLength       = imageResult.mDecompressedLength;
    length.mCompressedBufferLength  = imageResult.mCompressedLength;
    header.mImageBufferLength.emplace_back(length);
  }

  DBufferSerializer serializer{};
  serializer << header.mPtrPreviousFontAddress << header.mPtrNextFontAddress;
  serializer << header.mInformationBufferLength;
  serializer << header.mImageBufferLength;
  serializer << compressedJsonResult.mCompressedBuffer;
  for (const auto& imageBufferResult : compressedImageBufferList) { serializer << imageBufferResult.mCompressedBuffer; }

  QFile file(fmt::format("./{}_{}.dyFont", fontMetaInfo.fontName, fontMetaInfo.fontStyle).c_str());
  if (file.open(QIODevice::WriteOnly))
  {
    QDataStream out(&file);
    out.writeRawData(serializer.mBuffer.data(), serializer.mBuffer.size());
  }
  file.close();
  return true;
}

//!
//! Implementation
//!

DyFontAtlasGenerator::DyFontAtlasGenerator(QWidget *parent) : QMainWindow(parent)  // NOLINT
{
  ui.setupUi(this);
  // Initial setting
  this->setWindowFlag(Qt::WindowType::MSWindowsFixedSizeDialogHint);
  this->statusBar()->setSizeGripEnabled(false);
  this->statusBar()->hide();

  // Set visibility and enablility.
  ui.BT_Create->setEnabled(false);
  ui.PG_Loading->setVisible(false);

  // Connect signal and slot.
  connect(&this->mFutureWatcher,    SIGNAL(finished()),       this, SLOT(CreationTaskFinished()));
  connect(ui.BT_FindFile,           SIGNAL(clicked()),        this, SLOT(FindFontFile()));
  connect(ui.BT_FindTextFile,       SIGNAL(clicked()),        this, SLOT(FindTextFile()));

  connect(ui.CB_MapEnglish,         &QCheckBox::stateChanged, this, &DyFontAtlasGenerator::UpdateCharmapFlag);
  connect(ui.CB_MapCJKHanbun,       &QCheckBox::stateChanged, this, &DyFontAtlasGenerator::UpdateCharmapFlag);
  connect(ui.CB_MapHangul,          &QCheckBox::stateChanged, this, &DyFontAtlasGenerator::UpdateCharmapFlag);
  connect(ui.CB_MapKana,            &QCheckBox::stateChanged, this, &DyFontAtlasGenerator::UpdateCharmapFlag);
  connect(ui.CB_MapAutomatic,       &QCheckBox::stateChanged, this, &DyFontAtlasGenerator::UpdateCharmapFlag);

  connect(ui.CB_OptionSeperate,     &QCheckBox::stateChanged, this, &DyFontAtlasGenerator::UpdateOptionFlag);
  connect(ui.CB_OptionCompressJson, &QCheckBox::stateChanged, this, &DyFontAtlasGenerator::UpdateOptionFlag);
  connect(ui.BT_Create,             &QPushButton::clicked,    this, &DyFontAtlasGenerator::CreateBatchFile);
  //connect(ui.AC_About,            &QAction::triggered,      this, &DyFontAtlasGenerator::ShowAbout);

  connect(this, &DyFontAtlasGenerator::SetProgressBarValue, ui.PG_Loading, &QProgressBar::setValue);

  // Set callback to flag.
  this->mIsCanSelectCharmap.SetCallback(*this, &DyFontAtlasGenerator::UpdateFlags);
  this->mIsCanSelectWithTextGlyph.SetCallback(*this, &DyFontAtlasGenerator::UpdateTextFlag);
}

void DyFontAtlasGenerator::IncrementProgress()
{
  emit SetProgressBarValue(ui.PG_Loading->value() + 1);
}

void DyFontAtlasGenerator::FindFontFile()
{ 
  // Find font file using file explorer.
  const QString file = QFileDialog::getOpenFileName(
      this, tr("Open File"), "",
      tr("TrueType Collection (*.ttc);;TrueType Font (*.ttf);;OpenType Font (*.otf)"));;

  // If not given specified font file, just do nothing.
  if (file.isEmpty() == true) 
  { 
    ui.TV_FilePath->clear(); 
    this->mIsCanSelectCharmap = false;
  }
  else
  { //
    QFuture<DDyFontInformation> function1 = QtConcurrent::run(GetFontGeneralInformation, file);
    function1.waitForFinished();
    this->mFontInformation = function1.result();
    // 
    ui.TV_FilePath->setPlainText(fmt::format("Font Name : {}, Style : {}",
        this->mFontInformation.fontName,
        this->mFontInformation.fontStyle).c_str()
    );

    // Enable check box.
    this->mIsCanSelectCharmap = true;
  }
}

void DyFontAtlasGenerator::UpdateFlags(const bool& iNewValue)
{
  if (iNewValue == true)
  {
    ui.CB_MapCJKHanbun->setEnabled(true); ui.CB_MapEnglish->setEnabled(true);
    ui.CB_MapHangul->setEnabled(true);    ui.CB_MapKana->setEnabled(true);
    if (this->mIsCanSelectWithTextGlyph == true) 
    { 
      ui.CB_MapAutomatic->setEnabled(true); 
    }
  }
  else
  {
    ui.CB_MapCJKHanbun->setChecked(false);  ui.CB_MapCJKHanbun->setEnabled(false);
    ui.CB_MapEnglish->setChecked(false);    ui.CB_MapEnglish->setEnabled(false);
    ui.CB_MapHangul->setChecked(false);     ui.CB_MapHangul->setEnabled(false);
    ui.CB_MapKana->setChecked(false);       ui.CB_MapKana->setEnabled(false);
    ui.CB_MapAutomatic->setChecked(false);  ui.CB_MapAutomatic->setEnabled(false); 
  }
}

void DyFontAtlasGenerator::UpdateTextFlag(const bool& iNewValue)
{
  if (iNewValue == true && this->mIsCanSelectCharmap == true)
  {
    ui.CB_MapAutomatic->setEnabled(true);
  }
  else
  {
    ui.CB_MapAutomatic->setChecked(false); ui.CB_MapAutomatic->setEnabled(false);
  }
}

void DyFontAtlasGenerator::FindTextFile()
{
  // Find text file using file explorer.
  const QString file = QFileDialog::getOpenFileName(this, tr("Open Text File"), "", tr("Text file (*.txt);;"));

  // If not given specified text file, just do nothing but clear text glpyh set.
  if (file.isEmpty() == true) 
  { 
    ui.TV_TextFilePath->clear(); 
    this->mTextGlyphs.mCharGlyphs.clear();
  }
  else
  { 
    // Read file as UTF-8.
    QFuture<std::optional<DDyTextFileInformation>> function1 = QtConcurrent::run(GetTextGlyphs, file);
    function1.waitForFinished();

    // Check result is succeeded.
    const auto& optResult = function1.result();
    if (optResult.has_value() == false) 
    { 
      ui.TV_TextFilePath->clear(); 
      this->mIsCanSelectWithTextGlyph = false;
    }

    // Set up
    this->mTextGlyphs = *optResult;
    // Set plain text.
    ui.TV_TextFilePath->setPlainText(DyString(
        "{} glyphs will be created.", 
        this->mTextGlyphs.mCharGlyphs.size()).c_str()
    );
    this->mIsCanSelectWithTextGlyph = true;
  }
}

void DyFontAtlasGenerator::UpdateCharmapFlag([[maybe_unused]] int value)
{
  auto resultFlag {EDyCharmapCollections::None};
  if (ui.CB_MapCJKHanbun->isChecked() == true)  { resultFlag |= EDyCharmapCollections::CJK; }
  if (ui.CB_MapEnglish->isChecked() == true)    { resultFlag |= EDyCharmapCollections::English; }
  if (ui.CB_MapHangul->isChecked() == true)     { resultFlag |= EDyCharmapCollections::Hangul; }
  if (ui.CB_MapKana->isChecked() == true)       { resultFlag |= EDyCharmapCollections::Kana; }
  if (ui.CB_MapAutomatic->isChecked() == true)  { resultFlag |= EDyCharmapCollections::Automatic; }

  this->mCharmapFlag = resultFlag;
  if (this->mCharmapFlag != EDyCharmapCollections::None)  { ui.BT_Create->setEnabled(true); }
  else                                                    { ui.BT_Create->setEnabled(false); }
 }

void DyFontAtlasGenerator::UpdateOptionFlag(int value)
{
  auto resultFlag {EDyOptionCollections::None};
  if (ui.CB_OptionSeperate->isChecked() == true)     { resultFlag |= EDyOptionCollections::SeparateJsonAndPng; }
  if (ui.CB_OptionCompressJson->isChecked() == true) { resultFlag |= EDyOptionCollections::CompressJsonString_Deprecated; }

  this->mOptionFlag = resultFlag;
}

void DyFontAtlasGenerator::CreateBatchFile()
{
  static auto sIsMapInitialized {false};
  static auto sEnglishMap       {std::vector<FT_ULong>(ENGLISH_CHR_RANGE)};
  static auto sHangulMap        {std::vector<FT_ULong>(HANGUL_CHR_RANGE)};
  static auto sKanaMap          {std::vector<FT_ULong>(KANA_CHR_RANGE)};
  static auto sCJKHanbunMap     {std::vector<FT_ULong>(CJK_CHR_RANGE)};

  // First, initialize map charcode information.
  if (sIsMapInitialized == false)
  {
    std::generate(sEnglishMap.begin(), sEnglishMap.end(),     [n = ENGLISH_CHR_START]() mutable { return n++; });
    std::generate(sHangulMap.begin(), sHangulMap.end(),       [n = HANGUL_CHR_START]() mutable { return n++; });
    std::generate(sKanaMap.begin(), sKanaMap.end(),           [n = KANA_CHR_START]() mutable { return n++; });
    std::generate(sCJKHanbunMap.begin(), sCJKHanbunMap.end(), [n = CJK_CHR_START]() mutable { return n++; });
    sIsMapInitialized = true;
  }

  //! - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //! FunctionBody
  //! - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // Disable widget for malfunction.
  this->setEnabled(false);
  // Calculate chracter glyph map count to retrieve.
  int32_t               maxSize = 0;
  std::vector<FT_ULong> targetCharMap = {};

  if (IsHavingFlags(this->mCharmapFlag, EDyCharmapCollections::Automatic) == true)
  {
    maxSize += this->mTextGlyphs.mCharGlyphs.size();
    for (const auto& chrCode : this->mTextGlyphs.mCharGlyphs) { targetCharMap.emplace_back(chrCode); }
  }
  else
  {
    if (IsHavingFlags(this->mCharmapFlag, EDyCharmapCollections::English) == true)
    {
      maxSize += sEnglishMap.size();
      targetCharMap.insert(targetCharMap.end(), sEnglishMap.begin(), sEnglishMap.end());
    }
    if (IsHavingFlags(this->mCharmapFlag, EDyCharmapCollections::Hangul)  == true)
    {
      maxSize += sHangulMap.size();
      targetCharMap.insert(targetCharMap.end(), sHangulMap.begin(), sHangulMap.end());
    }
    if (IsHavingFlags(this->mCharmapFlag, EDyCharmapCollections::Kana)  == true)
    {
      maxSize += sKanaMap.size();
      targetCharMap.insert(targetCharMap.end(), sKanaMap.begin(), sKanaMap.end());
    }
    if (IsHavingFlags(this->mCharmapFlag, EDyCharmapCollections::CJK)  == true)
    {
      maxSize += sCJKHanbunMap.size();
      targetCharMap.insert(targetCharMap.end(), sCJKHanbunMap.begin(), sCJKHanbunMap.end());
    }
  }

  // Set progress bar status.
  ui.PG_Loading->setMinimum(0);
  ui.PG_Loading->setMaximum(maxSize);
  ui.PG_Loading->setValue(ui.PG_Loading->minimum());
  ui.PG_Loading->show();

  // Start the computation in other thread.
  CreateFontBuffer(this->mFontInformation, std::move(targetCharMap), this->mOptionFlag, *this);
  //const QFuture<void> future = QtConcurrent::run(CreateFontBuffer, this->mFontInformation,
      //std::move(targetCharMap),
      //this->mOptionFlag,
      //std::ref(*this));
  //this->mFutureWatcher.setFuture(future);

  this->CreationTaskFinished();
}

void DyFontAtlasGenerator::CreationTaskFinished()
{
  ui.PG_Loading->hide();
  this->setEnabled(true);
}

void DyFontAtlasGenerator::ShowAbout()
{ // NOT IMPLEMENTED YET
  this->mChildAbout = new DyWindowAbout();
  this->mChildAbout->setWindowModality(Qt::WindowModality::ApplicationModal);
  this->mChildAbout->SetParentMainWindow(*this);
  this->mChildAbout->show();
}

void DyFontAtlasGenerator::ResumeFocus()
{ // NOT IMPLEMENTED YET
  delete this->mChildAbout;
  this->mChildAbout = nullptr;
}

} /// dy namespace
