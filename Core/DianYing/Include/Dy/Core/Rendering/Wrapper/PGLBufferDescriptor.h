#ifndef GUARD_DY_CORE_RENDERING_WRAPPER_PDYGLBUFFERDESCRIPTOR_H
#define GUARD_DY_CORE_RENDERING_WRAPPER_PDYGLBUFFERDESCRIPTOR_H
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

#include <Dy/Meta/Information/MetaInfoModelMesh.h>

namespace dy
{

/// @enum EDirectBufferType
/// @brief direct buffer type value which can be shared with multiple OpenGL contexts.
enum class EDirectBufferType
{
  VertexBuffer,
  ElementBuffer,
  UniformBuffer,

  TransformFeedback,
  ShaderStorage,
  NoneError,
};

/// @struct PGLBufferDescriptor
/// @brief Descriptor instance for creating direct buffer context of opengl.
struct PGLBufferDescriptor final
{
  EDirectBufferType mBufferType     = EDirectBufferType::NoneError;
  TU32              mBufferByteSize = 0;
  const void*       mPtrBuffer      = nullptr;

  EDyMeshUsage      mBufferUsage    = EDyMeshUsage::StaticDraw;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RENDERING_WRAPPER_PDYGLBUFFERDESCRIPTOR_H