#ifndef GUARD_DY_CORE_RENDERING_HELPER_FRAMEATTACHMENTSTRING_H
#define GUARD_DY_CORE_RENDERING_HELPER_FRAMEATTACHMENTSTRING_H
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

#include <string_view>
#include <Dy/Helper/Macroes.h>

namespace dy
{

//!
//! Framebuffer
//!

MDY_SET_IMMUTABLE_STRING(sFrameBuffer_Deferred,     "dyBtBasicRender");
MDY_SET_IMMUTABLE_STRING(sFrameBuffer_Shadow,       "dyBtFbShadow");
MDY_SET_IMMUTABLE_STRING(sFrameBuffer_UiBasic,      "dyBtFbUiBasic");

//!
//! Attachment
//!

MDY_SET_IMMUTABLE_STRING(sAttachment_Unlit,         "dyBtUnlit");
MDY_SET_IMMUTABLE_STRING(sAttachment_Normal,        "dyBtNormal");
MDY_SET_IMMUTABLE_STRING(sAttachment_Specular,      "dyBtSpecular");
MDY_SET_IMMUTABLE_STRING(sAttachment_ModelPosition,  "dyBtModelPosition");

MDY_SET_IMMUTABLE_STRING(sAttachment_DirectionalBasicShadow, "dyBtAtDirBscShadow");

/// sFrameBuffer_UiBasic
MDY_SET_IMMUTABLE_STRING(sAttachment_Output,        "dyBtAtUiBasic_Output");

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RENDERING_HELPER_FRAMEATTACHMENTSTRING_H