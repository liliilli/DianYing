#ifndef GUARD_DY_META_TYPE_EDYRESOURCETYPES_H
#define GUARD_DY_META_TYPE_EDYRESOURCETYPES_H
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
/// @enum EDyResourceSource
/// @brief Source type of resource.
///
enum class EDyResourceSource
{ // This resource is builtin resource (internal).
  Builtin,
  // This resource is external resource.
  External,
};

///
/// @enum EDyGLShaderType
/// @brief
///
///
/// @enum EDyShaderFragmentType
/// @brief Shader fragment type. each fragment values are binded to each shader code prior to linking.
///
enum class EDyShaderFragmentType
{
  Vertex = 0,
  Hull = 1,
  Domain = 2,
  Geometry = 3,
  Pixel = 4,
  Compute = 5
};

} /// ::dy namespace

#endif /// GUARD_DY_META_TYPE_EDYRESOURCETYPES_H