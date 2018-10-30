#include "precompiled.h"
#include "DyFontAtlasGenerator.h"

#include <QtOpenGL>
#include <QtConcurrent/QtConcurrentRun>
#include <QThread>

#include <ft2build.h>
#if !defined(FT_FREETYPE_H)
#define FT_FREETYPE_H
#endif
#include <freetype/freetype.h>
#include <spdlog/fmt/fmt.h>

namespace
{

void DyInitializeFreetype(FT_Library& ftLibrary) noexcept
{ // Check Freetype is we`ll.
  if (FT_Init_FreeType(&ftLibrary) != 0) { }
}

void DyLoadFontFreetype(FT_Library& ftLibrary, FT_Face& ftFace, const QString fontPath) noexcept
{
  if (FT_New_Face(ftLibrary, fontPath.toStdString().c_str(), 0, &ftFace)) { }
}

void DyReleaseFreetype(FT_Library& ftLibrary, FT_Face& ftFace) noexcept
{
  if (FT_Done_Face(ftFace) != 0) { }
  if (FT_Done_FreeType(ftLibrary) != 0) { }

  ftFace = nullptr;
  ftLibrary = nullptr;
}

struct DDyFontInformation final
{
  std::string fontName  = "";
  std::string fontStyle = "";
};

DDyFontInformation HelloTestQtConcurrent(const QString fontPath)
{
  FT_Library sFtLibrary = nullptr;
  FT_Face    sFtFace    = nullptr;

  DyInitializeFreetype(sFtLibrary);
  DyLoadFontFreetype(sFtLibrary, sFtFace, fontPath);

  DDyFontInformation result = {};
  result.fontName   = sFtFace->family_name;
  result.fontStyle  = sFtFace->style_name;

  DyReleaseFreetype(sFtLibrary, sFtFace);
  return result;
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
  connect(ui.BT_FindFile, SIGNAL(clicked()), this, SLOT(FindFontFile()));
  connect(ui.CB_MapEnglish, &QCheckBox::stateChanged, this, &DyFontAtlasGenerator::UpdateCharmapFlag);
  connect(ui.CB_OptionSeperate, &QCheckBox::stateChanged, this, &DyFontAtlasGenerator::UpdateOptionFlag);

  // Call private trivial functions.
  this->pUpdateBT_CreateActivation();
}

void DyFontAtlasGenerator::FindFontFile()
{
  const QString file = QFileDialog::getOpenFileName(
      this,
      tr("Open File"),
      "",
      tr("TrueType Collection (*.ttc);;TrueType Font (*.ttf);;OpenType Font (*.otf)"));;

  if (file.isEmpty() == true) { ui.TV_FilePath->clear(); }
  else
  {
    ui.TV_FilePath->setPlainText(file);
    this->mFilePath = file;

    QFuture<DDyFontInformation> function1 = QtConcurrent::run(HelloTestQtConcurrent, this->mFilePath);
    function1.waitForFinished();
    const DDyFontInformation result = function1.result();
    ui.TV_FilePath->setPlainText(fmt::format("Font Name : {}, Style : {}", result.fontName, result.fontStyle).c_str());
  }
}

void DyFontAtlasGenerator::UpdateCharmapFlag(int value)
{
  dy::EDyCharmapCollections resultFlag = dy::EDyCharmapCollections::None;
  if (ui.CB_MapEnglish->isChecked() == true) { resultFlag |= dy::EDyCharmapCollections::English; }

  this->mCharmapFlag = resultFlag;
  this->pUpdateBT_CreateActivation();
 }

void DyFontAtlasGenerator::UpdateOptionFlag(int value)
{
  dy::EDyOptionCollections resultFlag = dy::EDyOptionCollections::None;
  if (ui.CB_OptionSeperate->isChecked() == true) { resultFlag |= dy::EDyOptionCollections::SeparateJsonAndPng; }

  this->mOptionFlag = resultFlag;
  this->pUpdateBT_CreateActivation();
}

void DyFontAtlasGenerator::pUpdateBT_CreateActivation()
{
  const bool resultFlag = false
    || dy::IsFlagNotCheckAll(this->mCharmapFlag)
    || dy::IsFlagNotCheckAll(this->mOptionFlag);

  if (resultFlag == true)  { ui.BT_Create->setEnabled(false); }
  else                     { ui.BT_Create->setEnabled(true); }
}

