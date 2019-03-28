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

#include <variant>
#include <nlohmann/json_fwd.hpp>
#include <Dy/Helper/Type/DVector2.h>
#include <Dy/Meta/Information/CommonResourceMetaInfo.h>
#include <Dy/Meta/Type/EDyTextureParameter.h>
#include <Dy/Core/Resource/Internal/ETextureEnums.h>
#include <Dy/Helper/Type/DColorRGBA.h>
#include <Dy/Helper/Internal/DImageBinaryBuffer.h>

namespace dy
{

///
/// @class PDyTextureInstanceMetaInfo
/// @brief Texture construction & meta descriptor.
///
struct PDyTextureInstanceMetaInfo final : public PDyCommonResourceMetaInfo
{
  struct DCubemapFilePath 
  {
    std::string mTopPath    = MDY_INITIALIZE_EMPTYSTR;
    std::string mBottomPath = MDY_INITIALIZE_EMPTYSTR;
    std::string mFrontPath  = MDY_INITIALIZE_EMPTYSTR;
    std::string mBackPath   = MDY_INITIALIZE_EMPTYSTR;
    std::string mRightPath  = MDY_INITIALIZE_EMPTYSTR;
    std::string mLeftPath   = MDY_INITIALIZE_EMPTYSTR;
  };

  /// Texture specification name.
  std::string         mSpecifierName    = MDY_INITIALIZE_EMPTYSTR;
  /// Texture external file path.
  /// DCubemapFilePath only applied when `mTextureType` is ETextureStyleType::D2Cubemap;
  std::variant<std::string, DCubemapFilePath> mExternalFilePath = MDY_INITIALIZE_EMPTYSTR;
  /// Texture is 1D or 2D?
  ETextureStyleType mTextureType = ETextureStyleType::NoneError;
  /// Border color
  DColorRGBA        mBorderColor = DColorRGBA::Black;
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
  DIVec2       mBuiltinBufferSize = {};
  /// Built-In buffer pointer of texture.
  std::vector<TU08>*  mPtrBuiltinBuffer = MDY_INITIALIZE_NULL;

  /// @deprecated NOT USED NOW
  std::string mTextureFileAbsolutePath_Deprecated = MDY_INITIALIZE_EMPTYSTR;
  /// @deprecated NOT USED NOW
  bool        mIsEnabledAbsolutePath_Deprecated = true;
  /// @deprecated NOT USED NOW.
  ETextureMapType   mTextureMapType_Deprecated = ETextureMapType::Unknown;
};

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyTextureInstanceMetaInfo& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyTextureInstanceMetaInfo& p);

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyTextureInstanceMetaInfo::DCubemapFilePath& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyTextureInstanceMetaInfo::DCubemapFilePath& p);

} /// ::dy namespace

#endif /// GUARD_DY_META_INFORMATION_METAINFOTEXTURE_H