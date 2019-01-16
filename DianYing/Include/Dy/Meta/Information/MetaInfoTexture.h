#ifndef GUARD_DY_META_INFORMATION_METAINFOTEXTURE_H
#define GUARD_DY_META_INFORMATION_METAINFOTEXTURE_H
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

#include <nlohmann/json_fwd.hpp>
#include <Dy/Core/Resource/Internal/TextureType.h>
#include <Dy/Helper/Type/VectorInt2.h>
#include <Dy/Meta/Information/CommonResourceMetaInfo.h>

namespace dy
{

///
/// @class PDyTextureInstanceMetaInfo
/// @brief Texture construction & meta descriptor.
///
struct PDyTextureInstanceMetaInfo final : public PDyCommonResourceMetaInfo
{
  /// Texture specification name.
  std::string         mSpecifierName    = MDY_INITIALIZE_EMPTYSTR;
  /// Texture external file path.
  std::string         mExternalFilePath = MDY_INITIALIZE_EMPTYSTR;
  /// Texture is 1D or 2D?
  EDyTextureStyleType mTextureType      = EDyTextureStyleType::NoneError;
  /// Border color
  DDyColorRGBA        mBorderColor      = DDyColorRGBA::Black;
  /// as R, RG, RGB, RGBA
  EDyImageColorFormatStyle  mTextureColorType = EDyImageColorFormatStyle::NoneError;
  /// 
  EDyGlImagePixelReadType   mPixelReadType = EDyGlImagePixelReadType::UnsignedByte;
  /// Texture parameters
  TTextureParameterList    mParameterList    = {};
  /// Use default opengl generated mipmap if true.
  bool                mIsUsingDefaultMipmapGeneration     = false;
  /// Use customized texture parameters (PDyTextureConstructionBaseDesc::mParameterList)
  bool                mIsEnabledCustomedTextureParameter  = false;
  /// Integer size builtin buffer container.
  DDyVectorInt2       mBuiltinBufferSize = {};
  /// Built-In buffer pointer of texture.
  std::vector<TU08>*  mPtrBuiltinBuffer = MDY_INITIALIZE_NULL;

  /// @deprecated NOT USED NOW
  std::string mTextureFileAbsolutePath_Deprecated = MDY_INITIALIZE_EMPTYSTR;
  /// @deprecated NOT USED NOW
  bool        mIsEnabledAbsolutePath_Deprecated = true;
  /// @deprecated NOT USED NOW.
  EDyTextureMapType   mTextureMapType_Deprecated = EDyTextureMapType::Custom;
};

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyTextureInstanceMetaInfo& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyTextureInstanceMetaInfo& p);

} /// ::dy namespace

#endif /// GUARD_DY_META_INFORMATION_METAINFOTEXTURE_H