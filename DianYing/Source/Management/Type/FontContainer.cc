#include <precompiled.h>
///
/// MIT License
/// Copyright (c) 2018 Jongmin Yun
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
#include <Dy/Management/Type/FontContainer.h>

#undef max
#undef min
#undef INFINITE
#if defined(_WIN32)
#pragma warning(push)
#pragma warning(disable:4244 4267 4458)
#endif
#include <msdfgen.h>
#if defined(_WIN32)
#pragma warning(pop)
#endif

#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftoutln.h>
#include FT_FREETYPE_H

//!
//! Forward declaration & local translation unit functions
//!

namespace
{

dy::DDyFontCharacter DyCreateUcs2CharTexture(_MIN_ const TC16 charCode);
dy::DDyFontCharacter DyCreateUcs2MSDFCharTexture(_MIN_ const TC16 charCode);
dy::DDyFontCharacter DyCreateUcs2SDFCharTexture(_MIN_ const TC16 charCode, _MIN_ FT_Face freetypeFace);

//!
//! Freetype pointer
//!

/// Freetype library pointer
FT_Library	sFreetypeLib  = nullptr;
/// Freetype face pointer used when initiating fonts.
FT_Face		  sFreetypeFace = nullptr;

///
/// @struct DDyFreeTypeContext
/// @brief
///
struct DDyFreeTypeContext
{
  msdfgen::Point2   mPosition;
  msdfgen::Shape*   mShape;
  msdfgen::Contour* mContour;
};

MDY_NODISCARD EDySuccess DyInitializeFreetype() noexcept
{
  // Check Freetype is we`ll.
  if (FT_Init_FreeType(&sFreetypeLib) != 0) {
    MDY_UNEXPECTED_BRANCH();
    return DY_FAILURE;
  }
  return DY_SUCCESS;
}

MDY_NODISCARD EDySuccess DyLoadFontFreetype(const std::string& font_path) noexcept
{
  if (FT_New_Face(sFreetypeLib, font_path.c_str(), 0, &sFreetypeFace)) {
    MDY_UNEXPECTED_BRANCH();
    return DY_FAILURE;
  }
  return DY_SUCCESS;
}

MDY_NODISCARD EDySuccess DyReleaseFreetype() noexcept
{
  if (FT_Done_Face(sFreetypeFace) != 0) { MDY_UNEXPECTED_BRANCH(); }
  if (FT_Done_FreeType(sFreetypeLib) != 0) { MDY_UNEXPECTED_BRANCH(); }

  sFreetypeFace = nullptr;
  sFreetypeLib  = nullptr;
  return DY_SUCCESS;
}

MDY_NODISCARD msdfgen::Point2 ftPoint2(_MIN_ const FT_Vector& vector)
{
  return msdfgen::Point2(vector.x / 64., vector.y / 64.);
}

MDY_NODISCARD int ftMoveTo(_MIN_ const FT_Vector* to, _MIN_ void* user)
{
  DDyFreeTypeContext *context  = reinterpret_cast<DDyFreeTypeContext*>(user);
  context->mContour   = &context->mShape->addContour();
  context->mPosition  = ftPoint2(*to);
  return 0;
}

MDY_NODISCARD int ftLineTo(_MIN_ const FT_Vector* to, _MIN_ void* user)
{
  DDyFreeTypeContext *context = reinterpret_cast<DDyFreeTypeContext*>(user);
  context->mContour->addEdge(new msdfgen::LinearSegment(context->mPosition, ftPoint2(*to)));
  context->mPosition = ftPoint2(*to);
  return 0;
}

MDY_NODISCARD int ftConicTo(_MIN_ const FT_Vector* control, _MIN_ const FT_Vector* to, _MIN_ void* user)
{
  DDyFreeTypeContext *context = reinterpret_cast<DDyFreeTypeContext*>(user);
  context->mContour->addEdge(new msdfgen::QuadraticSegment(context->mPosition, ftPoint2(*control), ftPoint2(*to)));
  context->mPosition = ftPoint2(*to);
  return 0;
}

MDY_NODISCARD int ftCubicTo(_MIN_ const FT_Vector* control1, _MIN_ const FT_Vector* control2, _MIN_ const FT_Vector* to, _MIN_ void* user)
{
  DDyFreeTypeContext *context = reinterpret_cast<DDyFreeTypeContext*>(user);
  context->mContour->addEdge(new msdfgen::CubicSegment(context->mPosition, ftPoint2(*control1), ftPoint2(*control2), ftPoint2(*to)));
  context->mPosition = ftPoint2(*to);
  return 0;
}

///
/// @brief
/// @return
///
MDY_NODISCARD std::unordered_map<TC16, dy::DDyFontCharacter> DyGetAsciiCharTextures()
{
  auto glyphs = std::unordered_map<TC16, dy::DDyFontCharacter>{};

  for (TC16 charCode = 0; charCode < 128; ++charCode)
  {
    //auto [_, isSucceeded] = glyphs.try_emplace(charCode, DyCreateUcs2CharTexture(charCode));
    //auto [_, isSucceeded] = glyphs.try_emplace(charCode, DyCreateUcs2MSDFCharTexture(charCode));
    auto [_, isSucceeded] = glyphs.try_emplace(charCode, DyCreateUcs2SDFCharTexture(charCode, sFreetypeFace));
    MDY_ASSERT(isSucceeded == true, "Failed to insert some character.");
  }

  return glyphs;
}

///
/// @brief
/// @param  charCode
/// @return
///
#ifdef false
MDY_NODISCARD dy::DDyFontCharacter DyCreateUcs2CharTexture(_MIN_ const TC16 charCode)
{
  if (FT_Load_Char(sFreetypeFace, charCode, FT_LOAD_RENDER) != 0) { MDY_UNEXPECTED_BRANCH(); return {}; }

  // Generate Textures
  TU32 textureId = MDY_INITIALIZE_DEFUINT;
  glGenTextures(1, &textureId);
  glBindTexture(GL_TEXTURE_2D, textureId);

  const auto width  = sFreetypeFace->glyph->bitmap.width;
  const auto height = sFreetypeFace->glyph->bitmap.rows;

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, sFreetypeFace->glyph->bitmap.buffer);

  // Set Texture Options
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  // Store character for later use
  glBindTexture(GL_TEXTURE_2D, 0);
  return dy::DDyFontCharacter
  {
      textureId,
      dy::DDyVectorInt2{static_cast<TI32>(width), static_cast<TI32>(height)},
      dy::DDyVectorInt2{sFreetypeFace->glyph->bitmap_left, sFreetypeFace->glyph->bitmap_top},
      static_cast<TU32>(sFreetypeFace->glyph->advance.x)
  };
}

MDY_NODISCARD dy::DDyFontCharacter DyCreateUcs2MSDFCharTexture(_MIN_ const TC16 charCode)
{
  if (FT_Load_Char(sFreetypeFace, charCode, FT_LOAD_NO_SCALE) != 0) { MDY_UNEXPECTED_BRANCH(); return {}; }

  msdfgen::Shape shape;
  shape.contours.clear();
  shape.inverseYAxis = false;

  DDyFreeTypeContext context = {}; context.mShape = &shape;
  FT_Outline_Funcs ftFunctions;
  {
    ftFunctions.move_to   = &ftMoveTo;
    ftFunctions.line_to   = &ftLineTo;
    ftFunctions.conic_to  = &ftConicTo;
    ftFunctions.cubic_to  = &ftCubicTo;
    ftFunctions.shift     = 0;
    ftFunctions.delta     = 0;

    const FT_Error error = FT_Outline_Decompose(&sFreetypeFace->glyph->outline, &ftFunctions, &context);
    MDY_ASSERT(error == 0, "");
  }

  struct { double l, b, r, t; } bounds =
  {
    std::numeric_limits<double>::max(),
    std::numeric_limits<double>::max(),
    std::numeric_limits<double>::lowest(),
    std::numeric_limits<double>::lowest()
  };
  shape.normalize();
  shape.bounds(bounds.l, bounds.b, bounds.r, bounds.t);
  shape.inverseYAxis = true;

  const double      pxRange = 4;
  msdfgen::Vector2  translate;
  msdfgen::Vector2  scale = 1;

  double l = bounds.l, b = bounds.b, r = bounds.r, t = bounds.t;
  msdfgen::Vector2 frame(64, 64);
  frame -= 2 * pxRange;

  if (l >= r || b >= t)             { l = 0, b = 0, r = 1, t = 1; }
  const msdfgen::Vector2 dims(r - l, t - b);

  if (dims.x * frame.y < dims.y * frame.x)
  {
    translate.set(.5 * (frame.x / frame.y * dims.y - dims.x) - l, -b);
    scale = frame.y / dims.y;
  }
  else
  {
    translate.set(-l, .5 * (frame.y / frame.x * dims.x - dims.y) - b);
    scale = frame.x / dims.x;
  }

  translate += pxRange / scale;
  const double range = pxRange / msdfgen::min(scale.x, scale.y);

  //                                 max angle
  //                                 image width, height
  //                                 range, scale, translation
  msdfgen::edgeColoringSimple(shape, 3.0);
  msdfgen::Bitmap<msdfgen::FloatRGB> msdf(64, 64);
  msdfgen::generateMSDF(msdf, shape, range, scale, translate);

  // Generate Textures
  TU32 textureId = MDY_INITIALIZE_DEFUINT;
  glGenTextures   (1, &textureId);
  glBindTexture   (GL_TEXTURE_2D, textureId);
  glTexImage2D    (GL_TEXTURE_2D, 0, GL_RGB, 64, 64, 0, GL_RGB, GL_FLOAT, &msdf(0, 0));
  // Set Texture Options
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  // Store character for later use
  glBindTexture   (GL_TEXTURE_2D, 0);

  const auto width  = sFreetypeFace->glyph->bitmap.width;
  const auto height = sFreetypeFace->glyph->bitmap.rows;
  return dy::DDyFontCharacter
  {
      textureId,
      dy::DDyVectorInt2{static_cast<TI32>(width), static_cast<TI32>(height)},
      dy::DDyVectorInt2{sFreetypeFace->glyph->bitmap_left, sFreetypeFace->glyph->bitmap_top},
      static_cast<TU32>(sFreetypeFace->glyph->advance.x)
  };
}
#endif

///
/// @brief
/// @param  charCode
/// @param  freetypeFace
/// @return
///
MDY_NODISCARD dy::DDyFontCharacter DyCreateUcs2SDFCharTexture(_MIN_ const TC16 charCode, _MIN_ FT_Face freetypeFace)
{
  if (FT_Load_Char(freetypeFace, charCode, FT_LOAD_NO_SCALE) != 0) { MDY_UNEXPECTED_BRANCH(); return {}; }

  msdfgen::Shape shape = {};
  shape.contours.clear();
  DDyFreeTypeContext context = {};
  context.mShape = &shape;

  FT_Outline_Funcs ftFunctions;
  {
    ftFunctions.move_to   = &ftMoveTo;
    ftFunctions.line_to   = &ftLineTo;
    ftFunctions.conic_to  = &ftConicTo;
    ftFunctions.cubic_to  = &ftCubicTo;
    ftFunctions.shift     = 0;
    ftFunctions.delta     = 0;

    const FT_Error error = FT_Outline_Decompose(&freetypeFace->glyph->outline, &ftFunctions, &context);
    MDY_ASSERT(error == 0, "");
  }
  const auto glyphScale = static_cast<float>(256) / freetypeFace->units_per_EM;

  struct { double l, b, r, t; } bounds =
  {
    std::numeric_limits<double>::max(),     std::numeric_limits<double>::max(),
    std::numeric_limits<double>::lowest(),  std::numeric_limits<double>::lowest()
  };
  shape.normalize();
  shape.bounds(bounds.l, bounds.b, bounds.r, bounds.t);
  shape.inverseYAxis = true;

  const double      pxRange = 4;
  msdfgen::Vector2  translate;
  msdfgen::Vector2  scale = 1;

  double l = bounds.l, b = bounds.b, r = bounds.r, t = bounds.t;
  msdfgen::Vector2 frame(64, 64);
  frame -= 2 * pxRange;

  if (l >= r || b >= t) { l = 0; b = 0; r = 1; t = 1; }
  const msdfgen::Vector2 dims(r - l, t - b);

  if (dims.x * frame.y < dims.y * frame.x)
  {
    translate.set(.5 * (frame.x / frame.y * dims.y - dims.x) - l, -b);
    scale = frame.y / dims.y;
  }
  else
  {
    translate.set(-l, .5 * (frame.y / frame.x * dims.x - dims.y) - b);
    scale = frame.x / dims.x;
  }

  translate += pxRange / scale;
  const double range = pxRange / msdfgen::min(scale.x, scale.y);

  msdfgen::edgeColoringSimple(shape, 3.0);
  msdfgen::Bitmap<float> sdf = {64, 64};
  msdfgen::generateSDF(sdf, shape, range, scale, translate);

  translate -= pxRange / scale;
  translate *= glyphScale;
  translate += pxRange / scale * glyphScale;

  // Generate Textures
  TU32 textureId = MDY_INITIALIZE_DEFUINT;
  glGenTextures (1, &textureId);
  glBindTexture (GL_TEXTURE_2D, textureId);
  glTexImage2D  (GL_TEXTURE_2D, 0, GL_RED, 64, 64, 0, GL_RED, GL_FLOAT, &sdf(0, 0));
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glBindTexture   (GL_TEXTURE_2D, 0);

  return dy::DDyFontCharacter
  {
      textureId,
      dy::DDyVectorInt2{
          static_cast<TI32>(freetypeFace->glyph->metrics.width * glyphScale),
          static_cast<TI32>(freetypeFace->glyph->metrics.height * glyphScale)
      },
      dy::DDyVectorInt2{
          static_cast<TI32>(freetypeFace->glyph->metrics.horiBearingX * glyphScale),
          static_cast<TI32>(freetypeFace->glyph->metrics.horiBearingY * glyphScale)
      },
      freetypeFace->glyph->metrics.horiAdvance * glyphScale / 64.0f,
      dy::DDyVector2{static_cast<TF32>(scale.x / glyphScale), static_cast<TF32>(scale.y / glyphScale)},
      dy::DDyVector2{static_cast<TF32>(translate.x * 16), static_cast<TF32>(translate.y * 16)}
  };
}

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy
{

FDyFontContainer::~FDyFontContainer() = default;
FDyFontContainer::FDyFontContainer(_MIN_ const std::string& fontFilePath)
{
  //
  this->mFontFilePath = fontFilePath;
  //
  if (DyInitializeFreetype() == DY_SUCCESS && DyLoadFontFreetype(this->mFontFilePath) == DY_SUCCESS)
  {
    //FT_Set_Pixel_Sizes(sFreetypeFace, 0, 64);

    this->mFontGlyphContainer     = DyGetAsciiCharTextures();
    const auto glyphScale = static_cast<float>(256) / sFreetypeFace->units_per_EM;
    this->mUnscaledLinefeedHeight = sFreetypeFace->glyph->metrics.height / 64 * glyphScale;
    MDY_CALL_ASSERT_SUCCESS(DyReleaseFreetype());
  }
}

bool FDyFontContainer::IsFontGlyphExist(_MIN_ const TC16 fontCode)
{
  return this->mFontGlyphContainer.find(fontCode) != this->mFontGlyphContainer.end();
}

EDySuccess FDyFontContainer::CreateFontGlyph(_MIN_ const TC16 fontCode)
{
  MDY_ASSERT(this->mFontFilePath.empty() == false, "Unexpected error occurred.");

  if (DyInitializeFreetype() == DY_SUCCESS && DyLoadFontFreetype(this->mFontFilePath) == DY_SUCCESS)
  {
    //FT_Set_Pixel_Sizes(sFreetypeFace, 0, 64);

    auto [_, isSucceeded] = this->mFontGlyphContainer.try_emplace(fontCode, DyCreateUcs2SDFCharTexture(fontCode, sFreetypeFace));
    MDY_ASSERT(isSucceeded == true, "");
    MDY_CALL_ASSERT_SUCCESS(DyReleaseFreetype());
    return DY_SUCCESS;
  }
  else
  {
    MDY_UNEXPECTED_BRANCH();
    return DY_FAILURE;
  }
}

const DDyFontCharacter& FDyFontContainer::operator[](const TC16 fontCode)
{
  return this->mFontGlyphContainer.at(fontCode);
}

TI32 FDyFontContainer::GetScaledLinefeedHeight(const TI32 fontSize) const noexcept
{
  return static_cast<TI32>(this->mUnscaledLinefeedHeight * fontSize);
}

} /// ::dy namespace