#ifndef GUARD_DY_ELEMENT_RENDERABLEOBJECT_H
#define GUARD_DY_ELEMENT_RENDERABLEOBJECT_H
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

#include <Dy/Element/AWorldObject.h>

namespace dy
{

/// @class ARenderableObject
/// @brief Renderable object type internally.
class ARenderableObject : public AWorldObject
{
public:
  ARenderableObject() = default;
  virtual ~ARenderableObject() = 0;

  /// @brief Render object internally. This function is not thread safe.
  virtual void Render() = 0;

  MDY_SET_CRC32_HASH_WITH_TYPE(ARenderableObject);
  MDY_SET_TYPEMATCH_FUNCTION(AWorldObject, ARenderableObject);
  MDY_ONLY_MOVEABLE_PROPERTIES_DEFAULT(ARenderableObject);
};

inline ARenderableObject::~ARenderableObject() = default;

} /// ::dy namespace

#endif /// GUARD_DY_ELEMENT_RENDERABLEOBJECT_H