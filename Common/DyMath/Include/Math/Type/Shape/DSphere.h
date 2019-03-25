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

#include <Math/Common/XGlobalMacroes.h>
#include <Math/Common/Inline/TGlobalTypes.inl>

namespace dy::math
{

/// @class DSphere
/// @brief Sphere class
template <typename TType> 
struct MATH_NODISCARD DSphere final 
{
public:
  static_assert(
    kCategoryOf<TType> == EValueCategory::Real, 
    "Failed to make DSphere, DSphere only supports Real type.");

  using TValueType = TType;


};

} /// ::dy::math namespace