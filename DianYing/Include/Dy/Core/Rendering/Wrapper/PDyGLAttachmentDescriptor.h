#ifndef GUARD_DY_CORE_RENDERING_WRAPPER_PDYGLATTACHMENTDESCRIPTOR_H
#define GUARD_DY_CORE_RENDERING_WRAPPER_PDYGLATTACHMENTDESCRIPTOR_H

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

#include <Dy/Helper/Type/ColorRGBA.h>
#include <Dy/Helper/Type/VectorInt2.h>
#include <Dy/Meta/Type/EDyTextureParameter.h>
#include <Dy/Meta/Type/EDyResourceTypes.h>

namespace dy
{

///
/// @struct PDyGLAttachmentDescriptor
/// @brief Descriptor instance for creating attachment context of opengl.
///
struct PDyGLAttachmentDescriptor final
{
  DDyVectorInt2                 mBufferSize     = {};
  DDyColorRGBA                  mBorderColor    = {};
  TTextureParameterList         mParameterList  = {};
  bool                          mIsUsingCustomizedParameter = false;
  EDyRenderBufferInternalFormat mBufferFormat   = EDyRenderBufferInternalFormat::NoneError;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RENDERING_WRAPPER_PDYGLATTACHMENTDESCRIPTOR_H