#ifndef GUARD_DY_CORE_COMPONENT_INTERNAL_TEXTURE_TYPE_H
#define GUARD_DY_CORE_COMPONENT_INTERNAL_TEXTURE_TYPE_H
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

#include <Dy/Helper/Type/Color.h>
#include <Dy/Core/Component/Internal/TextureEnums.h>
#include <Dy/Helper/Internal/ImageBinaryBuffer.h>

namespace dy
{

///
/// @class PDyTextureParameterOption
/// @brief Texture parameter option container such as MIN_FILTER MAG_FILTER WRAP_S WRAP_T etc.
///
struct PDyTextureParameterOption final
{
  PDyTextureParameterOption(uint32_t option, uint32_t value) : mOption{option}, mValue{value} {};

  const auto& GetOption() const noexcept  { return mOption; }
  const auto& GetValue() const noexcept   { return mValue; }

private:
  uint32_t mOption = 0;
  uint32_t mValue  = 0;
};

///
/// @struct PDyTextureConstructionBaseDesc
/// @brief Texture construction base to apply to all derived descriptor type.
///
struct PDyTextureConstructionBaseDesc
{ // Use default opengl generated mipmap if true.
  bool mIsUsingDefaultMipmapGeneration = false;
};

///
/// @class PDyTextureConstructionDescriptor
/// @brief Texture construction descriptor.
///
struct PDyTextureConstructionDescriptor final : public PDyTextureConstructionBaseDesc
{
  std::string mTextureName                                        = MDY_INITILAIZE_EMPTYSTR;
  std::string mTextureFileLocalPath                               = MDY_INITILAIZE_EMPTYSTR;
  std::string mTextureFileAbsolutePath                            = MDY_INITILAIZE_EMPTYSTR;
  DDyColor    mConstantBorderColor                                = DDyColor::Black;

  std::vector<PDyTextureParameterOption> mTextureParameterOptions = {};

  bool        mIsEnabledAbsolutePath                              = true;
  bool        mIsEnabledCustomedTextureParameter                  = false;
  bool        mIsEnabledCreateMipmap                              = false;
  EDyTextureStyleType mTextureType                                = EDyTextureStyleType::None;
  EDyTextureMapType   mTextureMapType                             = EDyTextureMapType::Custom;
};

///
/// @class PDyTextureConstructionBufferChunkDescriptor
/// @brief
///
struct PDyTextureConstructionBufferChunkDescriptor final : public PDyTextureConstructionBaseDesc
{
  std::string         mTextureName                                = MDY_INITILAIZE_EMPTYSTR;
  DDyColor            mConstantBorderColor                        = DDyColor::Black;
  /// Buffer pointer (not owned so must manage resources manually)
  void*               mBufferPtr                                  = MDY_INITIALIZE_NULL;
  /// Texture width size
  TU32                mWidth                                      = 1;
  /// Texture height size
  TU32                mHeight                                     = 1;
  /// Texture type
  EDyTextureStyleType mTextureType                                = EDyTextureStyleType::None;
  EDyTextureMapType   mTextureMapType                             = EDyTextureMapType::Custom;
  EDyImageColorFormatStyle mTextureColorType                      = EDyImageColorFormatStyle::NoneError;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_COMPONENT_INTERNAL_TEXTURE_TYPE_H