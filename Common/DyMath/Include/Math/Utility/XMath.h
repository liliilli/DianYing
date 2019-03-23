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

#include <Math/Common/TGlobalTypes.h>
#include <Math/Type/Math/DVector2.h>
#include <Math/Type/Math/DVector3.h>
#include <Math/Type/Math/DVector4.h>

namespace dy::math
{

/// @brief Pi(π)
template <typename TType>
constexpr TType kPi = TType(3.1415926535897932385L);

/// @brief 2*Pi (2π)
template <typename TType>
constexpr TType k2Pi = TType(3.1415926535897932385L) * TType(2);

/// @brief E (e)
template <typename TType>
constexpr TType kExp = TType(2.71828182845904523536L);

/// @brief Check float lhs is nearly equal to rhs.
/// floating-point number type like a float and double has a natural problem like
/// a floating-error such as 0.1 but 0.10000007, so you have to use this function
/// if you want to compare two floating points.
constexpr bool IsNearlyEqual(TF32 lhs, TF32 rhs, TF32 error_tolerance = 0.001f) noexcept
{
  return (rhs < lhs ? lhs - rhs : rhs - lhs) < error_tolerance;
}

/// @brief Check double lhs is nearly equal to rhs.
/// floating-point number type like a float and double has a natural problem like
/// a floating-error such as 0.1 but 0.10000007, so you have to use this function
/// if you want to compare two double points.
constexpr bool IsNearlyEqual(TF64 lhs, TF64 rhs, TF64 error_tolerance = 0.0001) noexcept
{
  return (rhs < lhs ? lhs - rhs : rhs - lhs) < error_tolerance;
}

/// @brief Check given two vector values are equal within given tolerance value. 
/// 
/// floating-point number type like a float and double has a natural problem like 
/// a floating-error such as 0.1 but 0.10000007, so you have to use this function 
/// if you want to compare two double points.
template <typename TLeft, typename TRight>
bool IsNearlyEqual(const DVector2<TLeft>& lhs, const DVector2<TRight>& rhs, TReal tolerance);

/// @brief Check given two vector values are equal within given tolerance value. 
/// 
/// floating-point number type like a float and double has a natural problem like 
/// a floating-error such as 0.1 but 0.10000007, so you have to use this function 
/// if you want to compare two double points.
template <typename TLeft, typename TRight>
bool IsNearlyEqual(const DVector3<TLeft>& lhs, const DVector3<TRight>& rhs, TReal tolerance);

/// @brief Check given two vector values are equal within given tolerance value. 
/// 
/// floating-point number type like a float and double has a natural problem like 
/// a floating-error such as 0.1 but 0.10000007, so you have to use this function 
/// if you want to compare two double points.
template <typename TLeft, typename TRight>
bool IsNearlyEqual(const DVector4<TLeft>& lhs, const DVector4<TRight>& rhs, TReal tolerance);

} /// ::dy::math namespace
#include <Math/Utility/Inline/XMath/IsNearlyEqual.inl>