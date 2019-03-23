#pragma once
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

#include <Math/Common/XGlobalUtilites.h>

namespace dy::math
{

template <typename TType, TI64 TStart, TI64 TEnd>
constexpr DClamp<TType, TStart, TEnd>::DClamp(TType value) noexcept
  : mValue{std::clamp(value, Cast<TType>(TStart), Cast<TType>(TEnd))}
{ }

template <typename TType, TI64 TStart, TI64 TEnd>
constexpr DClamp<TType, TStart, TEnd>& 
DClamp<TType, TStart, TEnd>::operator=(TType value) noexcept
{
  this->mValue = std::clamp(value, Cast<TType>(TStart), Cast<TType>(TEnd));
  return *this;
}

template <typename TType, TI64 TStart, TI64 TEnd>
constexpr DClamp<TType, TStart, TEnd>::operator TType() const noexcept
{
  return this->mValue;
}

template <typename TType, TI64 TStart, TI64 TEnd>
constexpr typename DClamp<TType, TStart, TEnd>::TValueType 
DClamp<TType, TStart, TEnd>::operator()() const noexcept
{
  return this->mValue;
}

template <typename TType, TI64 TStart, TI64 TEnd>
const typename DClamp<TType, TStart, TEnd>::TValueType* 
DClamp<TType, TStart, TEnd>::Data() const noexcept
{
  return &this->mValue;
}

} /// ::dy::math namespace