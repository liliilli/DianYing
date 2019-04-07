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

#include <cassert>

namespace dy::expr 
{

template <typename TType>
TType& ISingleton<TType>::GetInstance() noexcept
{
  static TType instance;
  return instance;
}

template <typename TType>
EDySuccess ISingleton<TType>::Initialize() noexcept
{
  assert(ISingleton<TType>::mIsInitialized == false);
  const auto flag = GetInstance().pfInitialize();

  bool oldValue = false;
  ISingleton<TType>::mIsInitialized.compare_exchange_weak(oldValue, true);
  return flag;
}

template <typename TType>
EDySuccess ISingleton<TType>::Release() noexcept
{
  assert(ISingleton<TType>::mIsInitialized == true);
  const auto flag = GetInstance().pfRelease();

  bool oldValue = true;
  ISingleton<TType>::mIsInitialized.compare_exchange_weak(oldValue, false);
  return flag;
}

template <typename TType>
bool ISingleton<TType>::IsInitialized() noexcept
{
  return ISingleton<TType>::mIsInitialized;
}

template <typename TType>
bool ISingleton<TType>::IsReleased() noexcept
{
  return !ISingleton<TType>::mIsInitialized;
}

} /// ::dy::expr namespace