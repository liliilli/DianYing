#ifndef GUARD_DY_ELEMENT_OBJECT_H
#define GUARD_DY_ELEMENT_OBJECT_H
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

#include <Dy/Element/Interface/IDyToString.h>

namespace dy
{

///
/// @class FDyObject
/// @brief all base type of all derived element types.
///
class FDyObject : public IDyToString
{
public:
  virtual ~FDyObject() = default;

  std::string ToString() override = 0;

protected:
  ///
  /// @brief Get present object name.
  /// @return present object name.
  ///
  [[nodiscard]] const std::string& pGetObjectName() const noexcept
  {
    return this->mObjectName;
  }

  ///
  /// @brief Set new object name.
  /// @param newObjectName Objec name to apply newly.
  ///
  void pSetObjectName(const std::string& newObjectName) noexcept
  {
    this->mObjectName = newObjectName;
  }

private:
  MDY_TRANSIENT std::string mObjectName = MDY_NOT_INITILAIZED_STR;
};

} /// ::dy namespace

#endif /// GUARD_DY_ELEMENT_OBJECT_H