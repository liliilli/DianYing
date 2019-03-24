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

#include <Expr/Inline/TExtendedTraits.inl>
namespace dy::expr
{

/// @brief Check given type is iterable.
/// @tparam TType target type
/// @reference https://stackoverflow.com/questions/13830158/check-if-a-variable-is-iterable
template <typename TType>
constexpr auto IsIterable = decltype(details::IsIterable_T::func<TType>(0))::value;

/// @brief Check given type has begin() and end() function.
/// @tparam TType target type
template <typename TType>
constexpr auto HasBeginEnd = decltype(details::HasBeginEnd_T::func<TType>(0))::value;

} /// ::dy::expr namespace