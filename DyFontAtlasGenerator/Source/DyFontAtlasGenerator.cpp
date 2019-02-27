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

#include <freetype/freetype.h>
#include <spdlog/fmt/fmt.h>
#include <nlohmann/json.hpp>

#include <Include/AConstantLangRange.h>
#include <Include/FTaskThread.h>
#include <Include/Structure.h>
#include <Include/Library/HelperString.h>
#include <Include/Library/HelperZlib.h>
#include <Include/Library/JsonTypeWriterHelper.h>
#include <Include/QtHelper/PaintSurface.h>
#include <Include/Type/DFileInformations.h>
#include <Include/Type/DResult.h>

#include <DyWindowAbout.h>

namespace dy
{

[[nodiscard]] bool ExportAsSeparateJsonAndPng (const DDyFontInformation& fontMetaInfo, const nlohmann::json& fontJsonInfo, const std::vector<QImage>& fontImage);
[[nodiscard]] bool ExportAsIntegratedFile     (const DDyFontInformation& fontMetaInfo, const nlohmann::json& fontJsonInfo, const std::vector<QImage>& fontImage);

/// @brief
[[nodiscard]] bool ExportAsSeparateJsonAndPng(
    const DDyFontInformation& fontMetaInfo,   
    const nlohmann::json& fontJsonInfo, 
    const std::vector<QImage>& fontImage);

[[nodiscard]] std::optional<DDyTextFileInformation> GetTextGlyphs(const QString& iFontPath)
{
  // Open file.
  const std::string fontPath = iFontPath.toStdString();
  // ReSharper disable CppDeprecatedEntity
  FILE* fdFile = fopen(fontPath.c_str(), "r");
  if (fdFile == nullptr) { return std::nullopt; }

  // Make staging buffer.
  std::fseek(fdFile, 0, SEEK_END);
  const auto fileByteSize = std::ftell(fdFile);
  std::fseek(fdFile, 0, SEEK_SET);

  // Read file until EOF.
  std::vector<char> buffer(fileByteSize);
  std::fread(buffer.data(), sizeof(char), fileByteSize, fdFile);
  assert(std::feof(fdFile));

  // Check.
  std::set<uint16_t> returnedCharGlyphs;  // NOLINT
  for (auto it = buffer.data(); *it != '\0'; it += DyGetByteOfUtf8Char(it))
  {
    const auto chr = DyGetRawUtf16CharacterFrom(it);
    if (chr != SURROGATE_SIGN) { returnedCharGlyphs.emplace(chr); }
  }

  // Close file descriptor.
  fclose(fdFile);
  return DDyTextFileInformation{returnedCharGlyphs};
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
  connect(ui.BT_FirstFindFile,      SIGNAL(clicked()),        this, SLOT(FindFirstFontFile()));
  connect(ui.BT_SecondFindFile,     SIGNAL(clicked()),        this, SLOT(FindSecondFontFile()));
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
  auto lock = std::lock_guard<std::mutex>(this->mMutexProgressIncrement);
  emit SetProgressBarValue(ui.PG_Loading->value() + 1);
}

void DyFontAtlasGenerator::FindFirstFontFile()  
{ 
  const auto flag = this->CreateFontFileInformation(0); 
  if (flag == false)
  {
    this->ui.TV_FirstFilePath->clear(); 
    this->mIsCanSelectCharmap = false;
  }
  else
  {
    this->ui.TV_FirstFilePath->setPlainText(DyString("Font Name : {}, Style : {}",
        this->mFontInformations[0].fontName,
        this->mFontInformations[0].fontStyle).c_str()
    );

    this->mIsCanSelectCharmap = true; // Enable check box.
  }
}

void DyFontAtlasGenerator::FindSecondFontFile() 
{ 
  const auto flag = this->CreateFontFileInformation(1); 
  if (flag == false)
  {
    this->ui.TV_SecondFilePath->clear(); 
    this->mIsSecondaryFontAvailable = false;
  }
  else
  {
    this->ui.TV_SecondFilePath->setPlainText(DyString("Font Name : {}, Style : {}",
        this->mFontInformations[1].fontName,
        this->mFontInformations[1].fontStyle).c_str()
    );
    this->mIsSecondaryFontAvailable = true;
  }
}

bool DyFontAtlasGenerator::CreateFontFileInformation(uint32_t id)
{
  // Find font file using file explorer.
  const QString file = QFileDialog::getOpenFileName(
      this, tr("Open File"), "",
      tr("TrueType Collection (*.ttc);;TrueType Font (*.ttf);;OpenType Font (*.otf)"));;

  // If not given specified font file, just do nothing.
  if (file.isEmpty() == true) { return false; }

  std::optional<DDyFontInformation> optResult = GetFontGeneralInformation(file);
  if(optResult.has_value() == false) { return false; }

  this->mFontInformations[id] = *optResult;
  return true;
}

[[nodiscard]] std::optional<DDyFontInformation> 
DyFontAtlasGenerator::GetFontGeneralInformation(const QString& iFontPath)
{
  FT_Library sFtLibrary = nullptr;
  FT_Face    sFtFace    = nullptr;

  if (FT_Init_FreeType(&sFtLibrary) != 0) { return std::nullopt; }
  if (FT_New_Face(sFtLibrary, iFontPath.toStdString().c_str(), 0, &sFtFace)) { return std::nullopt; }

  DDyFontInformation result;
  result.fontName  = sFtFace->family_name;
  result.fontStyle = sFtFace->style_name;
  result.fontPath  = iFontPath.toStdString();
  result.mLineFeedHeight        = sFtFace->height;
  result.mScaledLineFeedHeight  = sFtFace->size->metrics.height;

  if (FT_Done_Face(sFtFace) != 0)         { return std::nullopt; }
  if (FT_Done_FreeType(sFtLibrary) != 0)  { return std::nullopt; }
  return result;
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
    // Enable second font file path also.
    ui.TV_SecondFilePath->setEnabled(true); ui.BT_SecondFindFile->setEnabled(true);
  }
  else
  {
    // Disable all buttons.
    ui.CB_MapCJKHanbun->setChecked(false);  ui.CB_MapCJKHanbun->setEnabled(false);
    ui.CB_MapEnglish->setChecked(false);    ui.CB_MapEnglish->setEnabled(false);
    ui.CB_MapHangul->setChecked(false);     ui.CB_MapHangul->setEnabled(false);
    ui.CB_MapKana->setChecked(false);       ui.CB_MapKana->setEnabled(false);
    ui.CB_MapAutomatic->setChecked(false);  ui.CB_MapAutomatic->setEnabled(false); 
    // Clear editor text.
    ui.TV_SecondFilePath->clear();          ui.TV_SecondFilePath->setEnabled(false);
    ui.BT_SecondFindFile->setEnabled(false); this->mIsSecondaryFontAvailable = false;
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
  const QString file = QFileDialog::getOpenFileName(
      this, tr("Open Text File"), "", 
      tr("Text file (*.txt);;Csv UTF-8 file (*.csv);;"));

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
  static bool sIsMapInitialized = false;
  static std::vector<FT_ULong> sEnglishMap    = std::vector<FT_ULong>(ENGLISH_CHR_RANGE);
  static std::vector<FT_ULong> sHangulMap     = std::vector<FT_ULong>(HANGUL_CHR_RANGE);
  static std::vector<FT_ULong> sKanaMap       = std::vector<FT_ULong>(KANA_CHR_RANGE);
  static std::vector<FT_ULong> sCJKHanbunMap  = std::vector<FT_ULong>(CJK_CHR_RANGE);

  // First, initialize map charcode information.
  if (sIsMapInitialized == false)
  {
    std::generate(sEnglishMap.begin(), sEnglishMap.end(),     [n = ENGLISH_CHR_START]() mutable { return n++; });
    std::generate(sHangulMap.begin(), sHangulMap.end(),       [n = HANGUL_CHR_START]() mutable { return n++; });
    std::generate(sKanaMap.begin(), sKanaMap.end(),           [n = KANA_CHR_START]() mutable { return n++; });
    std::generate(sCJKHanbunMap.begin(), sCJKHanbunMap.end(), [n = CJK_CHR_START]() mutable { return n++; });
    sIsMapInitialized = true;
  }

  // Disable widget for malfunction.
  this->setEnabled(false);
  // Calculate chracter glyph map count to retrieve.
  std::set<FT_ULong> targetCharMap = {};

  if (IsHavingFlags(this->mCharmapFlag, EDyCharmapCollections::Automatic) == true)
  {
    for (const auto& chrCode : this->mTextGlyphs.mCharGlyphs) { targetCharMap.emplace(chrCode); }
  }
  if (IsHavingFlags(this->mCharmapFlag, EDyCharmapCollections::English) == true)
  {
    for (const auto& chrCode : sEnglishMap) { targetCharMap.emplace(chrCode); }
  }
  if (IsHavingFlags(this->mCharmapFlag, EDyCharmapCollections::Hangul)  == true)
  {
    for (const auto& chrCode : sHangulMap) { targetCharMap.emplace(chrCode); }
  }
  if (IsHavingFlags(this->mCharmapFlag, EDyCharmapCollections::Kana)  == true)
  {
    for (const auto& chrCode : sKanaMap) { targetCharMap.emplace(chrCode); }
  }
  if (IsHavingFlags(this->mCharmapFlag, EDyCharmapCollections::CJK)  == true)
  {
    for (const auto& chrCode : sCJKHanbunMap) { targetCharMap.emplace(chrCode); }
  }

  // Set progress bar status.
  ui.PG_Loading->setMinimum(0);
  ui.PG_Loading->setMaximum(static_cast<int32_t>(targetCharMap.size()));
  ui.PG_Loading->setValue(ui.PG_Loading->minimum());
  ui.PG_Loading->show();

  // Set final list of glyph
  std::vector<FT_ULong> finalCharCodeList = {};
  finalCharCodeList.insert(finalCharCodeList.end(), targetCharMap.begin(), targetCharMap.end());

  // Check secondary font is avilable.
  std::vector<DDyFontInformation> informations = {this->mFontInformations[0]};
  if (this->mIsSecondaryFontAvailable == true)
  {
    informations.emplace_back(this->mFontInformations[1]);
  }

  // Start the computation in other thread.
  const QString windowTitle = this->windowTitle();
  this->setWindowTitle(DyString("[1/2] Populating {} glyphs...", finalCharCodeList.size()).c_str());

  this->CreateFontBuffer(informations, finalCharCodeList, this->mOptionFlag);
  this->CreationTaskFinished();

  this->setWindowTitle(windowTitle);
}

/// @brief Create font buffer and export outside by following option.
void DyFontAtlasGenerator::CreateFontBuffer(
    const std::vector<DDyFontInformation>& information,
    const std::vector<FT_ULong>& targetCharMap,
    const EDyOptionCollections& option)
{
  /// @brief Calculate range for searching of each thread.
  static auto CalculateRangeTo = [](
      uint64_t charMapSize, 
      uint64_t threadCount, 
      std::vector<FTaskThread::TCharMapRangePair>& result)
  {
    result.reserve(threadCount);
    for (uint64_t threadId = 0; threadId < threadCount; ++threadId)
    {
      const auto begin = threadId * charMapSize / threadCount;
      const auto end   = (threadId + 1) * charMapSize / threadCount;
      result.emplace_back(std::make_pair(begin, end));
    }
  };

  // Get concurrent thread number.
  const uint32_t concurrentThreadNumber = std::thread::hardware_concurrency();
  const auto     targetCharMapSize      = static_cast<uint64_t>(targetCharMap.size());

  // Make json information instance for font glyphs.
  nlohmann::json jsonDescriptor;  
  // Insert meta information of specified font.
  DMeta metaInformation;
  metaInformation.mFontSpecifierName   = information.front().fontName;
  metaInformation.mFontStyleSpecifier  = information.front().fontStyle;
  metaInformation.mHoriLinefeed        = information.front().mLineFeedHeight;
  jsonDescriptor["Meta"] = metaInformation;

  // Set thread's result list and font glyph creation tasks of each spawned thread.
  std::vector<DResult>                        charResultList(targetCharMapSize);
  std::vector<FTaskThread::TCharMapRangePair> charRangeList = {};
  CalculateRangeTo(targetCharMapSize, concurrentThreadNumber, charRangeList);

  // Freetype initialization with the number of threads to instantiate.
  std::vector<FTaskThread> workerThreadInstances;
  std::vector<std::thread> workerThreads;
  workerThreadInstances.reserve(concurrentThreadNumber);
  workerThreads.reserve(concurrentThreadNumber);

  for (uint32_t threadId = 0; threadId < concurrentThreadNumber; ++threadId)
  {
    workerThreadInstances.emplace_back(information);

  }
  for (uint32_t threadId = 0; threadId < concurrentThreadNumber; ++threadId)
  {
    workerThreads.emplace_back(
        std::ref(workerThreadInstances[threadId]), 
        std::ref(charResultList), 
        std::cref(targetCharMap),
        charRangeList[threadId]
    );
  }
  for (uint32_t threadId = 0; threadId < concurrentThreadNumber; ++threadId)
  {
    workerThreads[threadId].join();
  }

  workerThreads.clear();
  workerThreadInstances.clear();
  
  // Make paint surface of off-screen rendering.
  CPaintSurface paintSurface;
  paintSurface.resize(TEXTURE_CANVAS_S, TEXTURE_CANVAS_T);
  paintSurface.InitializeContext();
  paintSurface.ClearSurface();

  // Create texture atlas.
  std::vector<QImage> drawnImageList {};
  this->setWindowTitle(DyString("[2/2] Rendering {} glyph of {}...", 0, targetCharMapSize).c_str());

  for (uint64_t id = 0u; id < targetCharMapSize; ++id)
  {
    // Get result from charRangeList.
    const auto& result = charResultList[id];
    jsonDescriptor["Characters"][DyString("{0}", result.mCharCode)] = result.mItemJsonAtlas;

    // Make QImage from Bitmap<float> and texture from QImage. (RVO guaranted)
    paintSurface.UpdateBufferInformation(result.mCoordinateBound);
    paintSurface.CreatePreviousBufferStateTexture();
    paintSurface.BindTexturePointer(result.mImageBuffer);
    paintSurface.render();

    // Update GUI
    this->IncrementProgress();
    this->setWindowTitle(DyString("[2/2] Rendering {} glyph of {}...", id, targetCharMapSize).c_str());

    // If we hit limit per one texture, initialize new texture to render more glyphs.
    if ((id + 1) % TEXTURE_MAPLIMIT == 0)
    { // Export off-screen texture buffer to png or file information (binary).
      drawnImageList.emplace_back(paintSurface.GetImageFromGLFBO());
      paintSurface.ClearSurface();
    }
  }

  drawnImageList.emplace_back(paintSurface.GetImageFromGLFBO());
  paintSurface.ClearSurface();

  if (IsHavingFlags(option, EDyOptionCollections::SeparateJsonAndPng) == true)
  {
    const auto isSuccessful = ExportAsSeparateJsonAndPng(information.front(), jsonDescriptor, drawnImageList);
    Q_ASSERT(isSuccessful == true);
  }
  else
  {
    const auto isSuccessful = ExportAsIntegratedFile(information.front(), jsonDescriptor, drawnImageList);
    Q_ASSERT(isSuccessful == true);
  }
}

bool ExportAsSeparateJsonAndPng(
    const DDyFontInformation& fontMetaInfo,   
    const nlohmann::json& fontJsonInfo, 
    const std::vector<QImage>& fontImage)
{
  const QString filename = DyString("./{}_{}.dyFntRes", fontMetaInfo.fontName, fontMetaInfo.fontStyle).c_str();
  if (QFile file(filename); file.open(QIODevice::WriteOnly))
  {
    const auto compressedJsonResult = zlib::CompressBuffer(fontJsonInfo);
    QDataStream stream(&file);
    stream.writeBytes(compressedJsonResult.mCompressedBuffer.data(), compressedJsonResult.mCompressedLength);
    file.close();
  }

  const auto imageListCount = fontImage.size();
  for (auto imageId = 0u; imageId < imageListCount; ++imageId)
  {
    const auto isSuccessful = fontImage[imageId].save(
        DyString("./{}_{}_{}.png", fontMetaInfo.fontName, fontMetaInfo.fontStyle, imageId).c_str(), "PNG"
    );
    Q_ASSERT(isSuccessful == true);
  }

  return true;
}

[[nodiscard]] bool ExportAsIntegratedFile(
    const DDyFontInformation& fontMetaInfo, 
    const nlohmann::json& fontJsonInfo, 
    const std::vector<QImage>& fontImage)
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
    std::vector<zlib::DZlibResult> resultList(imageBufferList.size());
    for (auto imageId = 0u; imageId < imageBufferList.size(); ++imageId)
    {
      resultList[imageId] = zlib::CompressBuffer(imageBufferList[imageId]);
    }
    return resultList;
  };

  const auto compressedJsonResult      = zlib::CompressBuffer(fontJsonInfo);
  const auto compressedImageBufferList = CompressImageBufferList(ConvertToQByteArrayBuffers(fontImage));

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
  DBufferSerializer serializer{};
  serializer << header.mPtrPreviousFontAddress << header.mPtrNextFontAddress;
  serializer << header.mInformationBufferLength;
  serializer << header.mImageBufferLength;
  serializer << compressedJsonResult.mCompressedBuffer;
  for (const auto& imageBufferResult : compressedImageBufferList) { serializer << imageBufferResult.mCompressedBuffer; }

  QFile file(DyString("./{}_{}.dyFont", fontMetaInfo.fontName, fontMetaInfo.fontStyle).c_str());
  if (file.open(QIODevice::WriteOnly))
  {
    QDataStream out(&file);
    out.writeRawData(serializer.mBuffer.data(), serializer.mBuffer.size());
  }
  file.close();
  return true;
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
