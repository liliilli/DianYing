#ifndef GUARD_DY_CORE_RENDERING_WRAPPER_PDYGLVAOBINDESCRIPTOR_H
#define GUARD_DY_CORE_RENDERING_WRAPPER_PDYGLVAOBINDESCRIPTOR_H
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

#include <Dy/Meta/Type/Mesh/DDyGLVaoBindInformation.h>

namespace dy
{

///
/// @struct PDyGLVaoBindDescriptor
/// @brief Descriptor instance for binding attribute properties for valid Vertex Array Object id.
///
struct PDyGLVaoBindDescriptor final
{
  TU32 mVaoId             = MDY_INITIALIZE_DEFUINT;
  TU32 mBoundVboId        = MDY_INITIALIZE_DEFUINT;
  TU32 mBoundEboId        = MDY_INITIALIZE_DEFUINT;
  DDyGLVaoBindInformation mAttributeInfo {};
  /// @brief Instancing ID will be used as binding number 1.
  std::optional<TU32> mInstancingVboId;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RENDERING_WRAPPER_PDYGLVAOBINDESCRIPTOR_H