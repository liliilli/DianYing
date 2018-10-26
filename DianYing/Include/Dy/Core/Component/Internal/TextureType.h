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
#include <Dy/Meta/Type/TextureParameter.h>

namespace dy
{

///
/// @struct PDyTextureConstructionBaseDesc
/// @brief Texture construction base to apply to all derived descriptor type.
///
struct PDyTextureConstructionBaseDesc
{
  /// Texture specification name.
  std::string mTextureSpecifierName       = MDY_INITILAIZE_EMPTYSTR;
  /// Texture parameters
  std::vector<PDyGlTexParameterInformation> mParameterList = {};
  /// Use customized texture parameters (PDyTextureConstructionBaseDesc::mParameterList)
  bool mIsEnabledCustomedTextureParameter = false;
  /// Use default opengl generated mipmap if true.
  bool mIsUsingDefaultMipmapGeneration    = false;
  /// Border color
  DDyColor mBorderColor           = DDyColor::Black;
};

///
/// @class PDyTextureConstructionDescriptor
/// @brief Texture construction descriptor.
///
struct PDyTextureConstructionDescriptor final : public PDyTextureConstructionBaseDesc
{
  ///
  std::string mTextureFileLocalPath       = MDY_INITILAIZE_EMPTYSTR;
  ///
  std::string mTextureFileAbsolutePath    = MDY_INITILAIZE_EMPTYSTR;
  ///
  bool        mIsEnabledAbsolutePath      = true;
  /// Texture is 1D or 2D?
  EDyTextureStyleType mTextureType        = EDyTextureStyleType::None;
  /// Map type @TODO NOT USED NOW.
  EDyTextureMapType   mTextureMapType     = EDyTextureMapType::Custom;
};

///
/// @class PDyTextureConstructionBufferChunkDescriptor
/// @brief
///
struct PDyTextureConstructionBufferChunkDescriptor final : public PDyTextureConstructionBaseDesc
{
  /// Buffer pointer (not owned so must manage resources manually)
  void*               mBufferPtr             = MDY_INITIALIZE_NULL;
  /// Texture width size
  TU32                mWidth                 = 1;
  /// Texture height size
  TU32                mHeight                = 1;
  /// Texture type
  EDyTextureStyleType mTextureType           = EDyTextureStyleType::None;
  /// Map type @TODO NOT USED NOW.
  EDyTextureMapType   mTextureMapType        = EDyTextureMapType::Custom;
  ///
  EDyImageColorFormatStyle mTextureColorType = EDyImageColorFormatStyle::NoneError;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_COMPONENT_INTERNAL_TEXTURE_TYPE_H