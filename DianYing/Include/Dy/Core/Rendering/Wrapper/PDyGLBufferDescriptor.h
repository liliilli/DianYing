#ifndef GUARD_DY_CORE_RENDERING_WRAPPER_PDYGLBUFFERDESCRIPTOR_H
#define GUARD_DY_CORE_RENDERING_WRAPPER_PDYGLBUFFERDESCRIPTOR_H
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

namespace dy
{

///
/// @enum EDyDirectBufferType
/// @brief direct buffer type value which can be shared with multiple OpenGL contexts.
///
enum class EDyDirectBufferType
{
  VertexBuffer,
  ElementBuffer,
  UniformBuffer,

  TransformFeedback,
  ShaderStorage,
  NoneError,
};

///
/// @struct PDyGLBufferDescriptor
/// @brief Descriptor instance for creating direct buffer context of opengl.
///
struct PDyGLBufferDescriptor final
{
  EDyDirectBufferType mBufferType     = EDyDirectBufferType::NoneError;
  TU32                mBufferByteSize = MDY_INITIALIZE_DEFUINT;
  const void*         mPtrBuffer      = MDY_INITIALIZE_NULL;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RENDERING_WRAPPER_PDYGLBUFFERDESCRIPTOR_H