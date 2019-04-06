#include <precompiled.h>
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

#include <Include/FTaskThread.h>
#include <Include/AConstantLangRange.h>
#include <freetype/ftoutln.h>

namespace dy
{

FTaskThread::FTaskThread(const std::vector<DDyFontInformation>& informations)
{
  this->AtomicInitializeFreetype(informations);

  this->ftFunctions.move_to = &ftMoveTo;   this->ftFunctions.line_to = &ftLineTo;
  this->ftFunctions.conic_to = &ftConicTo; this->ftFunctions.cubic_to = &ftCubicTo;
  this->ftFunctions.shift = 0;             this->ftFunctions.delta = 0;
}

FTaskThread::~FTaskThread()
{
  auto lock = std::lock_guard<std::mutex>(sFtCommonMutex);
  for (auto& [library, face] : this->mLocalFtInstance)
  {
    Q_ASSERT(library != nullptr); Q_ASSERT(face != nullptr);
    // Must be succeeded.
    if (FT_Done_Face(face) != 0)        { Q_ASSERT(false); }
    if (FT_Done_FreeType(library) != 0) { Q_ASSERT(false); }
  }
}

void FTaskThread::AtomicInitializeFreetype(const std::vector<DDyFontInformation>& informations) noexcept
{
  auto lock = std::lock_guard<std::mutex>(sFtCommonMutex);
  this->mLocalFtInstance.reserve(informations.size());

  for (const auto& information : informations)
  {
    // Initialization must be succeeded.
    FT_Library tempFtLibrary;
    if (FT_Init_FreeType(&tempFtLibrary) != 0) { assert(false); }
    FT_Face tempFtFace;
    FT_New_Face(tempFtLibrary, information.fontPath.c_str(), 0, &tempFtFace);

    // Insert. 
    this->mLocalFtInstance.emplace_back(tempFtLibrary, tempFtFace);
  }
}

void FTaskThread::operator()(
    std::vector<DResult>& charResultList, 
    const std::vector<FT_ULong>& charMapList,
    const TCharMapRangePair charRangePair)
{
  const auto itStart  = charMapList.cbegin() + charRangePair.first;
  const auto itEnd    = charMapList.cbegin() + charRangePair.second;

  // Loop.
  for (auto charCodeIt = itStart; charCodeIt != itEnd; ++charCodeIt)
  {
    // Check there is a glyph between given FT_Face list.
    FT_Face ftFace = this->mLocalFtInstance.front().second; // Default FT_Face when not found anything.
    for (auto& [_, ftFaceItem] : this->mLocalFtInstance)
    {
      const auto codeIndex = FT_Get_Char_Index(ftFaceItem, *charCodeIt);
      if (codeIndex != 0)
      {
        ftFace = ftFaceItem;
      }
    }

    // Load char.
    const FT_Error loadCharErrorFlag = FT_Load_Char(ftFace, *charCodeIt, FT_LOAD_NO_SCALE);
    Q_ASSERT(loadCharErrorFlag == 0);

    // Set outline information wrapper.
    msdfgen::Shape shape = {};
    DDyFreeTypeContext context = {}; context.mShape = &shape;
    // Get outline information.
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

    const auto id = static_cast<uint64_t>(std::distance(charMapList.cbegin(), charCodeIt));
    const auto texCoord = CreateCoordinateInformation(TEXTURE_CANVAS_S, TEXTURE_CANVAS_T, TEXTURE_SIZE_S, TEXTURE_SIZE_T, id);

    // Make json information and return.
    nlohmann::json jsonWrite;
    jsonWrite["Size"] = DDyVector2{ float(ftFace->glyph->metrics.width), float(ftFace->glyph->metrics.height) } * glyphScale;
    jsonWrite["HoriBearing"] = DDyVector2{ float(ftFace->glyph->metrics.horiBearingX), float(ftFace->glyph->metrics.horiBearingY) } *glyphScale;
    jsonWrite["Scale"] = DDyVector2{ float(alignedInfo.scale.x), float(alignedInfo.scale.y) } / glyphScale;
    jsonWrite["Translate"] = DDyVector2{ float(translate.x), float(translate.y) } *16;
    jsonWrite["TexCoordBox"] = texCoord;
    jsonWrite["HoriAdvance"] = ftFace->glyph->metrics.horiAdvance * glyphScale / 64.0;

    charResultList[id] = DResult{ texCoord, CreateQImageFromSDFBuffer(sdfFloatBuffer), jsonWrite, *charCodeIt};
  }
}

/// @brief
FTaskThread::DAlignedBBoxInfo FTaskThread::AlignSDFBBoxFrame(const DBound2D& bounds)
{
  const msdfgen::Vector2 dims = {bounds.r - bounds.l, bounds.t - bounds.b};
  msdfgen::Vector2 translate = {};
  msdfgen::Vector2 scale = {1, 1};
  msdfgen::Vector2 frame = {
      static_cast<double>(TEXTURE_SIZE_S),
      static_cast<double>(TEXTURE_SIZE_T)
  };
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

  const double range = TEXTURE_PXRANGE / msdfgen::min(scale.x, scale.y);
  return DAlignedBBoxInfo{range, scale, translate};
};

QImage FTaskThread::CreateQImageFromSDFBuffer(const TSdfType& buffer)
{
  // Create blank image buffer, S, T size and ~~RGB32~~ Format_Grayscale8.
  QImage imageBuffer = {TEXTURE_SIZE_S, TEXTURE_SIZE_T, QImage::Format::Format_Grayscale8};

  // Fille pixel information to QImage texel.
  for (auto y{ 0 }; y < TEXTURE_SIZE_T; ++y)
  {
    for (auto x{ 0 }; x < TEXTURE_SIZE_S; ++x)
    {
      // Gray-scalized value.
      const auto changedUCharValue = static_cast<uint8_t>(std::floor(std::max(buffer(x, y), 0.f) * 255));

      // Insert with QRgb (QRgb will be converted to grayscalized value).
      const QRgb value = qRgb(changedUCharValue, changedUCharValue, changedUCharValue);
      imageBuffer.setPixel(x, y, value);
    }
  }

  // Return it.
  return imageBuffer;
};

} /// ::dy namespace