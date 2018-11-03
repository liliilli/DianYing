#include "precompiled.h"
/// Header file
#include "DyFontAtlasGenerator.h"

#include <thread>

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

#include "Helper/MsdfgenHelper.h"
#include "Helper/JsonTypeWriterHelper.h"
#include "Helper/CoordinateBounds.h"
#include "Helper/HelperZlib.h"

#include "DyWindowAbout.h"
#include "Helper/PaintSurface.h"

namespace
{

//!
//!
//!

static constexpr auto STANDARD_UNITPEREM = 256;
static constexpr auto TEXTURE_CANVAS_S   = 1024;
static constexpr auto TEXTURE_CANVAS_T   = 1024;
static constexpr auto TEXTURE_SIZE_S     = 64;
static constexpr auto TEXTURE_SIZE_T     = 64;
static constexpr auto CHANNEL_LIMIT      = 4;
static constexpr auto TEXTURE_MAPLIMIT   = (TEXTURE_CANVAS_S / TEXTURE_SIZE_S) * (TEXTURE_CANVAS_T / TEXTURE_SIZE_T) * CHANNEL_LIMIT;
static constexpr auto TEXTURE_PXRANGE    = 12.0;

constexpr auto ENGLISH_UNI20_START = 0x0000;
constexpr auto ENGLISH_UNI20_END   = 0x007F;
constexpr auto ENGLISH_UNI20_RANGE = ENGLISH_UNI20_END - ENGLISH_UNI20_START + 1;

constexpr auto HANGUL_UNI20_START = 0xAC00;
constexpr auto HANGUL_UNI20_END   = 0xD7AF;
constexpr auto HANGUL_UNI20_RANGE = HANGUL_UNI20_END - HANGUL_UNI20_START + 1;

constexpr auto JAPANESE_UNI20_KANA_START = 0x3000;
constexpr auto JAPANESE_UNI20_KANA_END   = 0x30FF;
constexpr auto JAPANESE_UNI20_KANA_RANGE = JAPANESE_UNI20_KANA_END - JAPANESE_UNI20_KANA_START + 1;

auto sFtLibraryList {std::vector<FT_Library>{}};
auto sFtFaceList    {std::vector<FT_Face>{}};
std::atomic<bool> sFtIsInitiailzed {false};

using TSdfType = msdfgen::Bitmap<float>;

///
/// @struct DBounds
/// @brief
///
struct DBounds final
{
  double l = std::numeric_limits<double>::max();
  double b = std::numeric_limits<double>::max();
  double r = std::numeric_limits<double>::lowest();
  double t = std::numeric_limits<double>::lowest();
};

///
/// @struct DResult
/// @brief
///
struct DResult final
{
  dy::DDyCoordinateBounds mCoordinateBound;
  QImage                  mImageBuffer;
  nlohmann::json          mItemJsonAtlas;
  FT_ULong                mCharCode;
};

///
/// @struct DAlignedBBoxInfo
/// @brief
///
struct DAlignedBBoxInfo final
{
  double            range;
  msdfgen::Vector2  scale;
  msdfgen::Vector2  translate;
};

//!
//!
//!

///
/// @brief
/// @param value
///
void DyResizeFreetypeList(const int32_t value)
{
  Q_ASSERT(value > 0);
  Q_ASSERT(sFtIsInitiailzed.load() == false);

  sFtLibraryList.resize(value);
  sFtFaceList   .resize(value);
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
  auto imageBuffer {QImage{TEXTURE_SIZE_S, TEXTURE_SIZE_T, QImage::Format::Format_RGB32}};

  for (auto y{ 0 }; y < TEXTURE_SIZE_T; ++y)
  {
    for (auto x{ 0 }; x < TEXTURE_SIZE_S; ++x)
    {
      auto value{ QRgb{} };
      const auto changedUCharValue{ static_cast<uint8_t>(std::floor(std::max(buffer(x, y), 0.f) * 255)) };
      // Just one.
      value = qRgb(changedUCharValue, 0, 0);
      imageBuffer.setPixel(x, y, value);
    }
  }

  return imageBuffer;
};

///
/// @brief Bounds boundary veridity check.
/// @param bounds Bounds instance
///
void CheckAndChangeBounds(DBounds& bounds)
{
  if (bounds.l >= bounds.r || bounds.b >= bounds.t)
  {
    bounds.l = 0; bounds.b = 0; bounds.r = 1; bounds.t = 1;
  }
};

///
/// @brief
/// @param bounds
///
DAlignedBBoxInfo AlignSDFBBoxFrame(const DBounds& bounds)
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

DDyFontInformation GetFontGeneralInformation(const QString fontPath)
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

///
/// @brief
/// @param  ftOutlineFunc
/// @param  charCode
/// @param  ftFace
/// @param  id
/// @return
///
DResult CreateGlyphInformation(const FT_Outline_Funcs& ftOutlineFunc, const FT_ULong charCode, const FT_Face ftFace, int32_t id)
{
  {
    const FT_Error errorFlag = FT_Load_Char(ftFace, charCode, FT_LOAD_NO_SCALE);
    Q_ASSERT(errorFlag == 0);
  }

  auto shape  {msdfgen::Shape{}};
  auto context{dy::DDyFreeTypeContext{}}; context.mShape = &shape;

  {
    const auto errorFlag = FT_Outline_Decompose(&ftFace->glyph->outline, &ftOutlineFunc, &context);
    Q_ASSERT(errorFlag == 0);
  }

  // Set bounds
  auto bounds {DBounds{}};
  shape.normalize();
  shape.bounds(bounds.l, bounds.b, bounds.r, bounds.t);
  shape.inverseYAxis = true;
  CheckAndChangeBounds(bounds);
  // Set translate (non scaled)
  const auto alignedInfo {AlignSDFBBoxFrame(bounds)};

  // Generate information
  msdfgen::edgeColoringSimple(shape, 3.0);
  auto sdfFloatBuffer {TSdfType{TEXTURE_SIZE_S, TEXTURE_SIZE_T}};
  msdfgen::generateSDF(sdfFloatBuffer, shape, alignedInfo.range, alignedInfo.scale, alignedInfo.translate);

  // Realign and scale translate for saving.
  const auto glyphScale { static_cast<float>(STANDARD_UNITPEREM) / ftFace->units_per_EM };
  auto translate { alignedInfo.translate };
  translate -= TEXTURE_PXRANGE / alignedInfo.scale;
  translate *= glyphScale;
  translate += TEXTURE_PXRANGE / alignedInfo.scale * glyphScale;

  auto texCoord {dy::CreateCoordinateInformation(TEXTURE_CANVAS_S, TEXTURE_CANVAS_T, TEXTURE_SIZE_S, TEXTURE_SIZE_T, id)};

  // Make json information and return.
  nlohmann::json jsonWrite;
  jsonWrite["Size"]        = dy::DDyVector2{float(ftFace->glyph->metrics.width), float(ftFace->glyph->metrics.height)} * glyphScale;
  jsonWrite["HoriBearing"] = dy::DDyVector2{float(ftFace->glyph->metrics.horiBearingX), float(ftFace->glyph->metrics.horiBearingY)} * glyphScale;
  jsonWrite["Scale"]       = dy::DDyVector2{float(alignedInfo.scale.x), float(alignedInfo.scale.y)} / glyphScale;
  jsonWrite["Translate"]   = dy::DDyVector2{float(translate.x), float(translate.y)} * 16;
  jsonWrite["TexCoordBox"] = texCoord;
  jsonWrite["HoriAdvance"] = ftFace->glyph->metrics.horiAdvance * glyphScale / 64.0;

  return DResult{texCoord, CreateQImageFromSDFBuffer(sdfFloatBuffer), jsonWrite, charCode};
}

///
/// @brief
/// @param information
/// @param targetCharMap
///
void CreateFontBuffer(const DDyFontInformation information,
                      const std::vector<FT_ULong> targetCharMap,
                      const dy::EDyOptionCollections option,
                      DyFontAtlasGenerator& parent)
{
  static auto sFtFunctions          {FT_Outline_Funcs{}};
  static auto sIsFunctionInitialized{false};
  if (sIsFunctionInitialized == false)
  {
    sFtFunctions.move_to = &dy::ftMoveTo;
    sFtFunctions.line_to = &dy::ftLineTo;
    sFtFunctions.conic_to = &dy::ftConicTo;
    sFtFunctions.cubic_to = &dy::ftCubicTo;
    sFtFunctions.shift = 0;
    sFtFunctions.delta = 0;

    sIsFunctionInitialized = true;
  }

  //!
  //! Lambda functions
  //!

  ///
  /// @brief Store json file and pngs by following options...
  /// @param
  /// @param
  ///
  static auto ExportInformations = [&](const nlohmann::json& jsonInstance)
  {
    if (dy::IsHavingFlags(option, dy::EDyOptionCollections::CompressJsonString) == false)
    {
      const QString filename = fmt::format("./{}_{}.json", information.fontName, information.fontStyle).c_str();
      QFile file {filename};
      if (file.open(QIODevice::WriteOnly))
      {
        QTextStream stream(&file);
        stream << jsonInstance.dump(2).c_str() << endl;
      }
      file.close();
    }
    else
    {
      const QString filename = fmt::format("./{}_{}.dyFntRes", information.fontName, information.fontStyle).c_str();
      QFile file(filename);
      if (file.open(QIODevice::WriteOnly))
      {
        QDataStream stream(&file);
        auto buffer{ dy::zlib::CompressString(jsonInstance.dump(2)) };
        stream.writeBytes(buffer.c_str(), buffer.size());
      }
      file.close();
    }
  };

  ///
  /// @brief
  /// @param
  /// @param
  ///

  //!
  //! FunctionBody
  //!

  // Get concurrent thread number.
  const auto concurrentThreadNumber {std::thread::hardware_concurrency()};
  const auto targetCharMapSize      { targetCharMap.size() };

  //
  DyResizeFreetypeList(concurrentThreadNumber);
  DyInitializeFreetype();
  DyLoadFontFreetype  (information.fontPath.c_str());

  // Make json information instance for font glyphs.
  nlohmann::json jsonDescriptor;

  auto metaInformation{dy::DMeta{}};
  metaInformation.mFontSpecifierName   = information.fontName;
  metaInformation.mFontStyleSpecifier  = information.fontStyle;
  metaInformation.mHoriLinefeed        = sFtFaceList[0]->height;
  jsonDescriptor["Meta"] = metaInformation;

  CPaintSurface paintSurface;
  paintSurface.resize(TEXTURE_CANVAS_S, TEXTURE_CANVAS_T);
  paintSurface.InitializeContext();
  paintSurface.ClearSurface();

  auto drawnImageList   {std::vector<QImage>{}};
  auto charCount        {0};

  //
  std::vector<std::future<DResult>> threadResultList {};
  threadResultList.reserve(concurrentThreadNumber);

  const auto remainCharCount        {targetCharMapSize % concurrentThreadNumber};
  const auto firstIterationMapCount {targetCharMapSize - remainCharCount};

  for (auto i{0u}; i < firstIterationMapCount; i += concurrentThreadNumber)
  {
    // Issue task to parallel thread.
    for (auto j{0u}; j < concurrentThreadNumber; ++j)
    {
      auto result = std::async(
          std::launch::async,
          CreateGlyphInformation,
          std::cref(sFtFunctions), targetCharMap[i + j], sFtFaceList[j], charCount + j
      );
      threadResultList.emplace_back(std::move(result));
    }

    // Get result from future list and rendering.
    for (auto j{0u}; j < concurrentThreadNumber; ++j)
    {
      DResult result {threadResultList[j].get()};
      jsonDescriptor["Characters"][fmt::format("{0}", result.mCharCode)] = result.mItemJsonAtlas;

      // Make QImage from Bitmap<float> and texture from QImage. (RVO guaranted)
      paintSurface.UpdateBufferInformation(result.mCoordinateBound);
      paintSurface.CreatePreviousBufferStateTexture();
      paintSurface.BindTexturePointer(result.mImageBuffer);
      paintSurface.render();

      //
      charCount += 1;
      parent.IncrementProgress();

      if (charCount % TEXTURE_MAPLIMIT == 0)
      { // Export offscreen texture buffer to png or file information (binary).
        drawnImageList.emplace_back(paintSurface.GetImageFromGLFBO());
        paintSurface.ClearSurface();
      }
    }

    threadResultList.clear();
    threadResultList.reserve(concurrentThreadNumber);
  }

  // Issue task to parallel thread.
  for (auto j{ 0u }; j < remainCharCount; ++j)
  {
    auto result = std::async(
        std::launch::async,
        CreateGlyphInformation,
        std::cref(sFtFunctions), targetCharMap[firstIterationMapCount + j], sFtFaceList[j], charCount + j
    );
    threadResultList.emplace_back(std::move(result));
  }

  // Get result from future list and rendering.
  for (auto j{ 0u }; j < remainCharCount; ++j)
  {
    DResult result{ threadResultList[j].get() };
    jsonDescriptor["Characters"][fmt::format("{0}", result.mCharCode)] = result.mItemJsonAtlas;

    // Make QImage from Bitmap<float> and texture from QImage. (RVO guaranted)
    paintSurface.UpdateBufferInformation(result.mCoordinateBound);
    paintSurface.CreatePreviousBufferStateTexture();
    paintSurface.BindTexturePointer(result.mImageBuffer);
    paintSurface.render();

    //
    charCount += 1;
    parent.IncrementProgress();

    if (charCount % TEXTURE_MAPLIMIT == 0)
    { // Export offscreen texture buffer to png or file information (binary).
      drawnImageList.emplace_back(paintSurface.GetImageFromGLFBO());
      paintSurface.ClearSurface();
    }
  }

  threadResultList.clear();

#ifdef false
  for (const auto& charCode : targetCharMap)
  {
    const DResult result {CreateGlyphInformation(sFtFunctions, charCode, , charId)};
    jsonDescriptor["Characters"][fmt::format("{0}", charCode)] = result.mItemJsonAtlas;

    paintSurface.UpdateBufferInformation(result.mCoordinateBound);
    paintSurface.CreatePreviousBufferStateTexture();
    // Make QImage from Bitmap<float> and texture from QImage. (RVO guaranted)
    paintSurface.BindTexturePointer(result.mImageBuffer);
    paintSurface.render();

    charId += 1;
    //
    parent.IncrementProgress();

    // Export offscreen texture buffer to png or file information (binary).
    if (charId % TEXTURE_MAPLIMIT == 0)
    {
      drawnImageList.emplace_back(paintSurface.GetImageFromGLFBO());
      paintSurface.ClearSurface();
    }
  }
#endif
  drawnImageList.emplace_back(paintSurface.GetImageFromGLFBO());
  paintSurface.ClearSurface();

  {
    auto i{0};
    for (const QImage& image : drawnImageList)
    {
      i++;
      image.save(fmt::format("./{}_{}_{}.png", information.fontName, information.fontStyle, i).c_str(), "PNG");
    }
  }

  DyReleaseFreetype();
  ExportInformations(jsonDescriptor);
}

} /// unnamed namespace

//!
//! Implementation
//!

DyFontAtlasGenerator::DyFontAtlasGenerator(QWidget *parent) : QMainWindow(parent)
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
  connect(&this->mFutureWatcher,  SIGNAL(finished()),       this, SLOT(CreationTaskFinished()));
  connect(ui.BT_FindFile,         SIGNAL(clicked()),        this, SLOT(FindFontFile()));
  connect(ui.CB_MapEnglish,       &QCheckBox::stateChanged, this, &DyFontAtlasGenerator::UpdateCharmapFlag);
  connect(ui.CB_MapHangul,        &QCheckBox::stateChanged, this, &DyFontAtlasGenerator::UpdateCharmapFlag);
  connect(ui.CB_MapKana,          &QCheckBox::stateChanged, this, &DyFontAtlasGenerator::UpdateCharmapFlag);
  connect(ui.CB_OptionSeperate,   &QCheckBox::stateChanged, this, &DyFontAtlasGenerator::UpdateOptionFlag);
  connect(ui.CB_OptionCompressJson, &QCheckBox::stateChanged, this, &DyFontAtlasGenerator::UpdateOptionFlag);
  connect(ui.BT_Create,           &QPushButton::clicked,    this, &DyFontAtlasGenerator::CreateBatchFile);
  //connect(ui.AC_About,            &QAction::triggered,      this, &DyFontAtlasGenerator::ShowAbout);
  connect(this, &DyFontAtlasGenerator::SetProgressBarValue, ui.PG_Loading, &QProgressBar::setValue);

  // Call private trivial functions.
  this->pUpdateBT_CreateActivation();
}

void DyFontAtlasGenerator::IncrementProgress()
{
  emit SetProgressBarValue(ui.PG_Loading->value() + 1);
}

void DyFontAtlasGenerator::FindFontFile()
{ // Find font file using file explorer.
  const QString file = QFileDialog::getOpenFileName(
      this,
      tr("Open File"),
      "",
      tr("TrueType Collection (*.ttc);;TrueType Font (*.ttf);;OpenType Font (*.otf)"));;

  // If not given specified font file, just do nothing.
  if (file.isEmpty() == true) { ui.TV_FilePath->clear(); }
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
  }
}

void DyFontAtlasGenerator::UpdateCharmapFlag(int value)
{
  auto resultFlag {dy::EDyCharmapCollections::None};
  if (ui.CB_MapEnglish->isChecked() == true)  { resultFlag |= dy::EDyCharmapCollections::English; }
  if (ui.CB_MapHangul->isChecked() == true)   { resultFlag |= dy::EDyCharmapCollections::Hangul; }
  if (ui.CB_MapKana->isChecked() == true)     { resultFlag |= dy::EDyCharmapCollections::Kana; }

  this->mCharmapFlag = resultFlag;
  this->pUpdateBT_CreateActivation();
 }

void DyFontAtlasGenerator::UpdateOptionFlag(int value)
{
  auto resultFlag {dy::EDyOptionCollections::None};
  if (ui.CB_OptionSeperate->isChecked() == true)      { resultFlag |= dy::EDyOptionCollections::SeparateJsonAndPng; }
  if (ui.CB_OptionCompressJson->isChecked() == true)  { resultFlag |= dy::EDyOptionCollections::CompressJsonString; }

  this->mOptionFlag = resultFlag;
  this->pUpdateBT_CreateActivation();
}

void DyFontAtlasGenerator::pUpdateBT_CreateActivation()
{
  const bool resultFlag = false
    || (dy::IsFlagNotCheckAll(this->mCharmapFlag) == true)
    || (dy::IsHavingFlags(this->mOptionFlag, dy::EDyOptionCollections::SeparateJsonAndPng) == false);

  if (resultFlag == true)  { ui.BT_Create->setEnabled(false); }
  else                     { ui.BT_Create->setEnabled(true); }
}

void DyFontAtlasGenerator::CreateBatchFile()
{
  static auto sIsMapInitialized {false};
  static auto sEnglishMap       {std::vector<FT_ULong>(ENGLISH_UNI20_RANGE)};
  static auto sHangulMap        {std::vector<FT_ULong>(HANGUL_UNI20_RANGE)};
  static auto sKanaMap          {std::vector<FT_ULong>(JAPANESE_UNI20_KANA_RANGE)};

  // First, initialize map charcode information.
  if (sIsMapInitialized == false)
  {
    std::generate(sEnglishMap.begin(), sEnglishMap.end(), [n = ENGLISH_UNI20_START]() mutable { return n++; });
    std::generate(sHangulMap.begin(), sHangulMap.end(),   [n = HANGUL_UNI20_START]() mutable { return n++; });
    std::generate(sKanaMap.begin(), sKanaMap.end(),       [n = JAPANESE_UNI20_KANA_START]() mutable { return n++; });
    sIsMapInitialized = true;
  }

  //!
  //! FunctionBody∨
  //!

  // Disable widget for malfunction.
  this->setEnabled(false);

  // Calculate chracter glyph map count to retrieve.
  auto maxSize        {int32_t{}};
  auto targetCharMap  {std::vector<FT_ULong>{}};

  if (dy::IsHavingFlags(this->mCharmapFlag, dy::EDyCharmapCollections::English) == true)
  {
    maxSize += sEnglishMap.size();
    targetCharMap.insert(targetCharMap.end(), sEnglishMap.begin(), sEnglishMap.end());
  }
  if (dy::IsHavingFlags(this->mCharmapFlag, dy::EDyCharmapCollections::Hangul)  == true)
  {
    maxSize += sHangulMap.size();
    targetCharMap.insert(targetCharMap.end(), sHangulMap.begin(), sHangulMap.end());
  }
  if (dy::IsHavingFlags(this->mCharmapFlag, dy::EDyCharmapCollections::Kana)  == true)
  {
    maxSize += sKanaMap.size();
    targetCharMap.insert(targetCharMap.end(), sKanaMap.begin(), sKanaMap.end());
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
{
  // NOT IMPLEMENTED YET
  this->mChildAbout = new DyWindowAbout();
  this->mChildAbout->setWindowModality(Qt::WindowModality::ApplicationModal);
  this->mChildAbout->SetParentMainWindow(*this);
  this->mChildAbout->show();
}

void DyFontAtlasGenerator::ResumeFocus()
{
  // NOT IMPLEMENTED YET
  delete this->mChildAbout;
  this->mChildAbout = nullptr;
}

