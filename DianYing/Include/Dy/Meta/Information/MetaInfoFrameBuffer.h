#ifndef GUARD_DY_META_INFORMATION_METAINFOFRAMEBUFFER_H
#define GUARD_DY_META_INFORMATION_METAINFOFRAMEBUFFER_H
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
#include <Dy/Management/Type/AttachmentInformation.h>
#include <Dy/Helper/Type/VectorInt2.h>

namespace dy
{

///
/// @struct PDyGlFrameBufferInstanceMetaInfo
/// @brief Meta information for constructing Frame buffer object.
///
struct PDyGlFrameBufferInstanceMetaInfo final : public PDyCommonResourceMetaInfo
{
  std::string             mSpecifierName    = MDY_INITIALIZE_EMPTYSTR;
  TAttachmentBinderList   mColorAttachmentList   = {};
  DDyVectorInt2           mFrameBufferSize  = {};

  std::string             mDepthAttachmentSpecifier = MDY_INITIALIZE_EMPTYSTR;
  bool                    mIsUsingDepthBuffer       = true;
  bool                    mIsNotUsingPixelShader    = false;
};

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyGlFrameBufferInstanceMetaInfo& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyGlFrameBufferInstanceMetaInfo& p);

} /// ::dy namespace

#endif /// GUARD_DY_META_INFORMATION_METAINFOFRAMEBUFFER_H