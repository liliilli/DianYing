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

/// @class FCallStack
/// @brief Stack that have callback function having TType value as a input parameter.
template <typename TType>
class FCallStack final
{
public:
  /// @brief Raw (C-style and static) function pointer type.
  using TRawCallback = void(*)(const TType&);
  /// @brief Member (C++ class, struct non-static) function type.
  template <typename TClass>
  using TMemberCallback = void(TClass::*)(const TType&);

  /// @brief Set callback function with raw type.
  void SetCallback(TRawCallback iPtrCallback); 

  /// @brief member-function overloaded version of `SetCallback` plain function.
  template <typename TClass>
  void SetCallback(TClass& iRefClass, TMemberCallback<TClass> iPtrMemberCallback);

  /// @brief Call callback function.
  /// But callback function is not bound or stack is empty, do nothing.
  void CallCallback();

  /// @brief Reset callback function to null, only if exist.
  void ResetCallback() { this->mCallback = nullptr; }

  /// @brief Push value into stack. 
  /// If callback function is exist but second parameter value is false, do not call function.
  /// Otherwise, just call function with inserted value.
  void Push(TType&& ioValue, bool iCallCallback = true);

  /// @brief Push value into stack. 
  /// If callback function is exist but second parameter value is false, do not call function.
  /// Otherwise, just call function with inserted value.
  void Push(const TType& ioValue, bool iCallCallback = true);

  /// @brief Get top value as not modifiable. \n
  /// This function must be called only if stack is not empty, so check by using `IsEmpty()` function
  /// whether it is empty or not.
  MDY_NODISCARD const TType& Top() const;

  /// @brief Extract top value and return value.
  /// If callback function is exist but first parameter value is false, do not call function.
  /// Otherwise, just call function with next top value.
  MDY_NODISCARD TType ExtractTop(bool iCallCallback = true);

  /// @brief Pop top value but does not return popped value.
  /// If callback function is exist but first parameter value is false, do not call function.
  /// Otherwise, just call function with next top value.
  void Pop(bool iCallCallback = true);

  /// @brief Check stack is empty.
  MDY_NODISCARD bool IsEmpty() const noexcept;

private:
  std::stack<TType> mInternalStack{};
  std::function<void(const TType&)> mCallback = nullptr;
};

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_INTERNAL_FDyCallStack_H
#include <Dy/Helper/Internal/Inline/FCallStackTmp.inl>