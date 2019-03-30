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

namespace dy::math
{

template <typename TType, typename>
constexpr TType ToNormalizedRadian(TType radian)
{
  if (radian >= -kPi<TType> && radian < kPi<TType>) { return radian; }

  if (std::signbit(radian) == true)
  {
    const auto quat = static_cast<TI32>((radian + kPi<TType>) / k2Pi<TType>);
    return radian + (quat * k2Pi<TType>);
  }
  else
  {
    const auto quat = static_cast<TI32>((radian - kPi<TType>) / k2Pi<TType>);
    return radian - (quat * k2Pi<TType>);
  }
}

template <typename TType, typename>
constexpr TType ToNormalizedDegree(TType degree)
{
  if (degree >= -180 && degree < 180) { return degree; }

  if (std::signbit(degree) == true)
  {
    const auto quat = static_cast<TI32>((degree + 180) / TType(360));
    return degree + (quat * TType(360));
  }
  else
  {
    const auto quat = static_cast<TI32>((degree - 180) / TType(360));
    return degree - (quat * TType(360));
  }
}

} /// ::dy::math namespace