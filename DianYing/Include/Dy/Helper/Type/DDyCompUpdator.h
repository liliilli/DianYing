#include <utility>
#ifndef GUARD_DY_Helper_Type_DDyCompUpdator_H
#define GUARD_DY_Helper_Type_DDyCompUpdator_H
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

namespace dy
{

/// @brief This type has callback function, lazy flag and value. \n
/// If lazy flag is false, when value is changed, callback function is called when callback is bound.
template <typename TType> 
class DDyCompUpdator final 
{
public:
  using TUnaryPredicate = std::function<void(const TType&)>;

  DDyCompUpdator() = default;
  DDyCompUpdator(const TType& iValue, TUnaryPredicate mCallback) : mValue{iValue}, mCallbackFunction{ std::move(mCallback) } {};

  /// @brief Set value
  DDyCompUpdator& operator=(const TType& iNewValue)
  {
    // Check validation
    if (this->mValue == iNewValue) { return *this; }
    // Set
    this->mValue = iNewValue;
    // Call ( not change this->mChanged ) 
    if (this->mCallbackFunction != nullptr) { this->mCallbackFunction(this->mValue); return *this; }
    // Else
    return *this;
  }

  /// @brief Try update (call callback function) when changed.
  /// If no value is inserted, reset callback function of this instance.
  void SetCallbackFunction(const TUnaryPredicate& iNewCallback = nullptr)
  {
    this->mCallbackFunction = iNewCallback;
  }

private:
  TType mValue = TType{};
  TUnaryPredicate mCallbackFunction = nullptr;
};

} /// ::dy namespace

#endif /// GUARD_DY_Helper_Type_DDyCompUpdator_H