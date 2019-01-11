#ifndef GUARD_DY_CORE_RENDERING_WRAPPER_PDYGLFRAMEBUFFERDESCRIPTOR_H
#define GUARD_DY_CORE_RENDERING_WRAPPER_PDYGLFRAMEBUFFERDESCRIPTOR_H
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

#include <Dy/Management/Type/AttachmentInformation.h>
#include <Dy/Core/Resource/Internal/TextureEnums.h>

namespace dy
{

struct PDyGLFrameBufferDescriptor final
{
  using TIsRenderBuffer     = bool;
  using TAttachmentBinding  = std::tuple<TU32, EDyTextureStyleType, EDyGlAttachmentType, TIsRenderBuffer>;
  using TAttachmentBindingList = std::vector<TAttachmentBinding>;

  DDyVectorInt2           mFrameBufferSize = {};
  TAttachmentBindingList  mAttachmentBindingList = {};
  TAttachmentBinding      mDepthBufferBinding = {};
  bool                    mIsUsingDepthBuffer = true;
  bool                    mIsUsingPixelShader = false;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RENDERING_WRAPPER_PDYGLFRAMEBUFFERDESCRIPTOR_H