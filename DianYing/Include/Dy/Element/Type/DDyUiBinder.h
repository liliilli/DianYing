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
class FDyUiObject;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

/// @class DDyUiBinder
/// @brief Manages Ui object more securely.
class DDyUiBinder final
{
public:
  DDyUiBinder() = default;
  explicit DDyUiBinder(FDyUiObject& bindObject);

  ~DDyUiBinder() noexcept;
  DDyUiBinder(const DDyUiBinder& mRefBinder) noexcept;
  DDyUiBinder& operator=(const DDyUiBinder& mRefBinder) noexcept;

  /// @brief UI Object is valid and exist.
  MDY_NODISCARD bool IsUiObjectValid() const noexcept { return this->mPtrUiObject != nullptr; }
  /// @brief Get object.
  MDY_NODISCARD FDyUiObject* Get() noexcept { return this->mPtrUiObject; }
  MDY_NODISCARD const FDyUiObject* Get() const noexcept { return this->mPtrUiObject; }

  FDyUiObject*        operator->() noexcept        { return this->mPtrUiObject; }
  const FDyUiObject*  operator->() const noexcept  { return this->mPtrUiObject; }

  FDyUiObject&        operator*() noexcept        { return *this->mPtrUiObject; }
  const FDyUiObject&  operator*() const noexcept  { return *this->mPtrUiObject; }

  /// @brief
  void MDY_PRIVATE(DetachUiObjectFromUiObject)();

private:
  void pAttachBinderToUiObject();
  void pDetachBinderFromUiObject();

  FDyUiObject* mPtrUiObject = nullptr;
};

static_assert(std::is_destructible_v<DDyUiBinder> == true, "");

} /// ::dy namespace

#endif // !GUARD_DY_ELEMENT_TYPE_DDyUiBinder_H
