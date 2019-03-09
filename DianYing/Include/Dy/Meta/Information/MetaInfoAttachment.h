#ifndef GUARD_DY_META_INFORMATION_METAINFOATTACHMENT_H
#define GUARD_DY_META_INFORMATION_METAINFOATTACHMENT_H
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

#include <Dy/Meta/Information/CommonResourceMetaInfo.h>
#include <Dy/Meta/Type/EDyTextureParameter.h>
#include <Dy/Helper/Type/VectorInt2.h>
#include <Dy/Helper/Type/ColorRGBA.h>
#include <Dy/Core/Resource/Internal/TextureEnums.h>
#include <Dy/Helper/Type/Clamp.h>

namespace dy
{

///
/// @struct PDyGlAttachmentInstanceMetaInfo
/// @brief Meta information for constructing attachment (texture object or render buffer) for FBO.
///
struct PDyGlAttachmentInstanceMetaInfo final : public PDyCommonResourceMetaInfo
{
  std::string                   mSpecifierName  = MDY_INITIALIZE_EMPTYSTR;
  TTextureParameterList         mParameterList  = {};
  DDyVectorInt2                 mAttachmentSize = {};
  DDyColorRGBA                  mBorderColor    = DDyColorRGBA::Black;
  EDyGlBufferDataInternalFormat mBufferFormat   = EDyGlBufferDataInternalFormat::NoneError;
  EDyTextureStyleType           mAttachmentType = EDyTextureStyleType::D2;
  DDyClamp<TU32, 1, 16>         mMipmapLevels   = 1;
  /// @brief This variable is only enabled if only mAttachmentType is `Array` type.
  TU32                          mDepthNumber    = 0;
  /// @brief When enabled, attachment will be created as ping-pong (two-attachment) attachment.
  bool                          mIsPingpong     = false;
};

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyGlAttachmentInstanceMetaInfo& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyGlAttachmentInstanceMetaInfo& p);

} /// ::dy namespace

#endif /// GUARD_DY_META_INFORMATION_METAINFOATTACHMENT_H