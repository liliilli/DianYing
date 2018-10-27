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
#include <ft2build.h>
#include FT_FREETYPE_H

//!
//! Forward declaration & local translation unit functions
//!

namespace
{

dy::DDyFontCharacter DyCreateUcs2CharTexture(_MIN_ const TC16 charCode);

//!
//! Freetype pointer
//!

/// Freetype library pointer
FT_Library	sFreetypeLib  = nullptr;
/// Freetype face pointer used when initiating fonts.
FT_Face		  sFreetypeFace = nullptr;

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

MDY_NODISCARD std::unordered_map<TC16, dy::DDyFontCharacter> GetAsciiCharTextures()
{
  auto glyphs = std::unordered_map<TC16, dy::DDyFontCharacter>{};

  for (TC16 charCode = 0; charCode < 128; ++charCode)
  {
    auto [_, isSucceeded] = glyphs.try_emplace(charCode, DyCreateUcs2CharTexture(charCode));
    MDY_ASSERT(isSucceeded == true, "Failed to insert some character.");
  }

  return glyphs;
}

MDY_NODISCARD dy::DDyFontCharacter DyCreateUcs2CharTexture(_MIN_ const TC16 charCode)
{
  if (FT_Load_Char(sFreetypeFace, charCode, FT_LOAD_RENDER) != 0)
  {
    MDY_UNEXPECTED_BRANCH();
    return {};
  }

  // Generate Textures
  TU32 textureId = MDY_INITIALIZE_DEFUINT;
  glGenTextures(1, &textureId);
  glBindTexture(GL_TEXTURE_2D, textureId);

  const auto width  = sFreetypeFace->glyph->bitmap.width;
  const auto height = sFreetypeFace->glyph->bitmap.rows;

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, sFreetypeFace->glyph->bitmap.buffer);

  // Set Texture Options
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
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
    FT_Set_Pixel_Sizes(sFreetypeFace, 0, 64);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    this->mFontGlyphContainer = GetAsciiCharTextures();
    this->mUnscaledLinefeedHeight = sFreetypeFace->size->metrics.height;
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
    FT_Set_Pixel_Sizes(sFreetypeFace, 0, 64);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    auto[_, isSucceeded] = this->mFontGlyphContainer.try_emplace(fontCode, DyCreateUcs2CharTexture(fontCode));
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

TI32 FDyFontContainer::GetScaledLinefeedHeight(const TF32 scaleOffset) const noexcept
{
  return static_cast<TI32>(this->mUnscaledLinefeedHeight * scaleOffset) >> 6;
}

} /// ::dy namespace