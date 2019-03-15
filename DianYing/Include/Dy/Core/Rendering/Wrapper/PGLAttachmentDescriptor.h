#ifndef GUARD_DY_CORE_RENDERING_WRAPPER_PDYGLATTACHMENTDESCRIPTOR_H
#define GUARD_DY_CORE_RENDERING_WRAPPER_PDYGLATTACHMENTDESCRIPTOR_H

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

#include <Dy/Helper/Type/DColorRGBA.h>
#include <Dy/Helper/Type/DVectorInt2.h>
#include <Dy/Meta/Type/EDyTextureParameter.h>
#include <Dy/Meta/Type/EDyResourceTypes.h>
#include <Dy/Core/Resource/Information/FDyAttachmentInformation.h>

namespace dy
{

/// @struct PGLAttachmentDescriptor
/// @brief Descriptor instance for creating attachment context of opengl.
struct PGLAttachmentDescriptor final
{
  DVectorInt2                   mBufferSize     = {};
  DColorRGBA                    mBorderColor    = {};
  TTextureParameterList         mParameterList  = {};
  bool                          mIsUsingCustomizedParameter = false;
  EGlBufferDataInternalFormat mBufferFormat   = EGlBufferDataInternalFormat::NoneError;
  ETextureStyleType           mAttachmentType = ETextureStyleType::NoneError;
  TU32                          mSpecifiedMipmapLevel = 1;
  // @brief This variable is only applied into attachment creation when `mAttachmentType` is `Array` only.
  TU32                          mDepthNumber    = 0;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RENDERING_WRAPPER_PDYGLATTACHMENTDESCRIPTOR_H