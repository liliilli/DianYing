#ifndef GUARD_DY_HELPER_INTERNAL_FDyCallStack_H
#define GUARD_DY_HELPER_INTERNAL_FDyCallStack_H
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

#include <stack>
#include <functional>

namespace dy
{

/// @class FDyCallVar
/// @brief Variable that have callback function having TType value as a input parameter.
template <typename TType>
class FDyCallVar final
{
public:
  /// @brief Raw (C-style and static) function pointer type.
  using TRawCallback = void(*)(const TType&);
  /// @brief Member (C++ class, struct non-static) function type.
  template <typename TClass>
  using TMemberCallback = void(TClass::*)(const TType&);

  /// @brief Set callback function with raw type.
  void SetCallback(TRawCallback iPtrCallback) { this->mCallback = iPtrCallback; }

  /// @brief member-function overloaded version of `SetCallback` plain function.
  template <typename TClass>
  void SetCallback(TClass& iRefClass, TMemberCallback<TClass> iPtrMemberCallback)
  {
    using namespace std::placeholders;
    this->mCallback = std::bind(iPtrMemberCallback, &iRefClass, _1);
  }

  /// @brief Call callback function.
  /// But callback function is not bound or stack is empty, do nothing.
  void CallCallback()
  {
    if (this->mCallback != nullptr) { this->mCallback(this->mInternalVar); }
  }

  /// @brief Reset callback function to null, only if exist.
  void ResetCallback() { this->mCallback = nullptr; }

  FDyCallVar() = default;
  FDyCallVar& operator=(const TType& iValue)
  {
    this->mInternalVar = iValue;
    this->CallCallback();
    return *this;
  }

  [[nodiscard]] bool operator==(const TType& iValue) const 
  {
    return mInternalVar == iValue;
  }

private:
  TType mInternalVar = {};
  std::function<void(const TType&)> mCallback = nullptr;
};

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_INTERNAL_FDyCallStack_H
