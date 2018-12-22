#ifndef GUARD_DY_HELPER_SYSTEM_TYPETRAITS_H
#define GUARD_DY_HELPER_SYSTEM_TYPETRAITS_H
///
/// MIT License
/// Copyright (c) 2018 Jongmin Yun
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.
///

#include <type_traits>

namespace dy
{

template <typename TDerived, typename TBase>
constexpr auto IsBaseClassOf = std::is_base_of_v<TBase, TDerived>;

template <typename TSource, typename TTarget>
constexpr auto IsSameClass = std::is_same_v<TSource, TTarget>;

template <typename TDerived, typename TBase>
constexpr auto IsInheritancedFrom = (IsBaseClassOf<TDerived, TBase> == true) && (IsSameClass<TDerived, TBase> == false);

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_SYSTEM_TYPETRAITS_H