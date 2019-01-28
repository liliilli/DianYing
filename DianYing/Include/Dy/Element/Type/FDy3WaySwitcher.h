#ifndef GUARD_DY_ELEMENT_TYPE_FDY3WaySwitcher_H
#define GUARD_DY_ELEMENT_TYPE_FDY3WaySwitcher_H
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

#include <Dy/Helper/Type/ThreeStateBool.h>

namespace dy
{

/// @class FDy3WaySwitcher
/// @brief Helper type for moduling 3-way state bool mechanism between hierarchical tree.
class FDy3WaySwitcher 
{
public:
  FDy3WaySwitcher() = default;
  FDy3WaySwitcher(const FDy3WaySwitcher&) = default;
  FDy3WaySwitcher& operator=(const FDy3WaySwitcher&) = default;
  FDy3WaySwitcher(FDy3WaySwitcher&&) = default;
  FDy3WaySwitcher& operator=(FDy3WaySwitcher&&) = default;
  virtual ~FDy3WaySwitcher() = default;

  /// @brief Activate binded instance, But bounce instance must be activated finally,
  /// following by parent state flag.
  void Activate();

  /// @brief Deactivate binded instance.
  void Deactivate();

  /// @brief Check switch is activated finally.
  MDY_NODISCARD bool IsActivated() const noexcept;

  /// @brief Setup parent flag. bounce instance will be activated finally, following by final state boolean value.
  void SetupFlagAsParent(_MIN_ bool iIsActivated);

protected:
  /// @brief This function must be implemented. 
  /// This function will be called when `Activate()` or `SetupFlagAsParent` called, 
  /// and final state boolean value is true from false.
  virtual void TryActivateInstance() = 0;
  
  /// @brief This function must be implemented.
  /// This function will be called when `Deactivate()` or `SetupFlagAsParent(false)` called,
  /// and final state boolean value is false from true.
  virtual void TryDeactivateInstance() = 0;

private:
  /// Activate flag for operating component.
  DDy3StateBool mActivateFlag = {};
};

}

#endif /// !GUARD_DY_ELEMENT_TYPE_FDY3WaySwitcher_H
