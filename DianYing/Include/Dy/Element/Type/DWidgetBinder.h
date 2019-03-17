#ifndef GUARD_DY_ELEMENT_TYPE_DDyUiBinder_H
#define GUARD_DY_ELEMENT_TYPE_DDyUiBinder_H
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

//!
//! Forward declaration
//!

namespace dy 
{
class AWidgetObject;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

/// @class DWidgetBinder
/// @brief Manages Ui object more securely.
class DWidgetBinder final
{
public:
  DWidgetBinder() = default;
  explicit DWidgetBinder(AWidgetObject& bindObject);

  ~DWidgetBinder() noexcept;
  DWidgetBinder(const DWidgetBinder& mRefBinder) noexcept;
  DWidgetBinder& operator=(const DWidgetBinder& mRefBinder) noexcept;

  /// @brief UI Object is valid and exist.
  MDY_NODISCARD bool IsUiObjectValid() const noexcept { return this->mPtrUiObject != nullptr; }
  /// @brief Get object.
  MDY_NODISCARD AWidgetObject* Get() noexcept { return this->mPtrUiObject; }
  MDY_NODISCARD const AWidgetObject* Get() const noexcept { return this->mPtrUiObject; }

  AWidgetObject*        operator->() noexcept        { return this->mPtrUiObject; }
  const AWidgetObject*  operator->() const noexcept  { return this->mPtrUiObject; }

  AWidgetObject&        operator*() noexcept        { return *this->mPtrUiObject; }
  const AWidgetObject&  operator*() const noexcept  { return *this->mPtrUiObject; }

  /// @brief
  void MDY_PRIVATE(DetachUiObjectFromUiObject)();

private:
  void pAttachBinderToUiObject();
  void pDetachBinderFromUiObject();

  AWidgetObject* mPtrUiObject = nullptr;
};

static_assert(std::is_destructible_v<DWidgetBinder> == true, "");

} /// ::dy namespace

#endif // !GUARD_DY_ELEMENT_TYPE_DDyUiBinder_H
