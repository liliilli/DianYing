#ifndef GUARD_DY_ELEMENT_OBJECT_H
#define GUARD_DY_ELEMENT_OBJECT_H
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

#include <Dy/Element/Interface/IToString.h>
#include <Dy/Helper/System/HashCompileCrc32.h>

namespace dy
{

/// @class AWorldObject
/// @brief all base type of all derived element types.
class AWorldObject : public IToString
{
public:
  virtual ~AWorldObject() = default;

  std::string ToString() override = 0;

protected:
  /// @brief Get present object name.
  /// @return present object name.
  [[nodiscard]] const std::string& pGetObjectName() const noexcept
  {
    return this->mObjectName;
  }

  /// @brief Set new object name.
  /// @param newObjectName Objec name to apply newly.
  void pSetObjectName(const std::string& newObjectName) noexcept
  {
    this->mObjectName = newObjectName;
  }

  /// @brief  Type match for static casting of instance in runtime
  /// @param  hashVal CRC32 hashValue
  /// @return If type is matched return true but false.
  virtual bool IsTypeMatched(const TU32 hashVal) const noexcept
  {
    return this->__mHashVal == hashVal;
  }

private:
  MDY_TRANSIENT std::string mObjectName = MDY_INITIALIZE_EMPTYSTR;
  MDY_SET_CRC32_HASH_WITH_TYPE(FDyObject);
};

} /// ::dy namespace

#endif /// GUARD_DY_ELEMENT_OBJECT_H