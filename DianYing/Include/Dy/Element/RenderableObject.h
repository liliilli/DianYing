#ifndef GUARD_DY_ELEMENT_RENDERABLEOBJECT_H
#define GUARD_DY_ELEMENT_RENDERABLEOBJECT_H
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

#include <Dy/Element/Object.h>

namespace dy
{

///
/// @class FDyRenderableObject
/// @brief Renderable object type internally.
///
class FDyRenderableObject : public FDyObject
{
public:
  FDyRenderableObject() = default;
  virtual ~FDyRenderableObject() = default;

  ///
  /// @brief Render object internally. This function is not thread safe.
  ///
  virtual void Render() = 0;

  MDY_SET_CRC32_HASH_WITH_TYPE(FDyRenderableObject);
  MDY_SET_TYPEMATCH_FUNCTION(FDyObject, FDyRenderableObject);
  MDY_ONLY_MOVEABLE_PROPERTIES_DEFAULT(FDyRenderableObject);
};

} /// ::dy namespace

#endif /// GUARD_DY_ELEMENT_RENDERABLEOBJECT_H