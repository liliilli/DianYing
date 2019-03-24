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

namespace dy::expr
{

template <typename TContainers> struct TTupleIterator;
template <typename TContainers> struct TTupleConstIterator;

template <typename TType> 
struct TTupleIterator<TTypeListTail<TType>> final
{
  using Type = std::tuple<
    details::TItemIterator<std::remove_reference_t<TType>>
  >;
};

template <typename _0, typename _1>
struct TTupleIterator<TTypeList<_0, TTypeListTail<_1>>> final
{
  using Type = std::tuple<
    details::TItemIterator<std::remove_reference_t<_0>>, 
    details::TItemIterator<std::remove_reference_t<_1>>
  >;
};

template <typename _0, typename _1, typename _2>
struct TTupleIterator<TTypeList<_0, TTypeList<_1, TTypeListTail<_2>>>> final
{
  using Type = std::tuple<
    details::TItemIterator<std::remove_reference_t<_0>>, 
    details::TItemIterator<std::remove_reference_t<_1>>,
    details::TItemIterator<std::remove_reference_t<_2>>
  >;
};

template <typename _0, typename _1, typename _2, typename _3>
struct TTupleIterator<TTypeList<_0, TTypeList<_1, TTypeList<_2, TTypeListTail<_3>>>>> final
{
  using Type = std::tuple<
    details::TItemIterator<std::remove_reference_t<_0>>, 
    details::TItemIterator<std::remove_reference_t<_1>>,
    details::TItemIterator<std::remove_reference_t<_2>>,
    details::TItemIterator<std::remove_reference_t<_3>>
  >;
};

template <typename TType> 
struct TTupleConstIterator<TTypeListTail<TType>> final
{
  using Type = std::tuple<
    details::TItemConstIterator<std::remove_reference_t<TType>>
  >;
};

template <typename _0, typename _1>
struct TTupleConstIterator<TTypeList<_0, TTypeListTail<_1>>> final
{
  using Type = std::tuple<
    details::TItemConstIterator<std::remove_reference_t<_0>>, 
    details::TItemConstIterator<std::remove_reference_t<_1>>
  >;
};

template <typename _0, typename _1, typename _2>
struct TTupleConstIterator<TTypeList<_0, TTypeList<_1, TTypeListTail<_2>>>> final
{
  using Type = std::tuple<
    details::TItemConstIterator<std::remove_reference_t<_0>>, 
    details::TItemConstIterator<std::remove_reference_t<_1>>,
    details::TItemConstIterator<std::remove_reference_t<_2>>
  >;
};

template <typename _0, typename _1, typename _2, typename _3>
struct TTupleConstIterator<TTypeList<_0, TTypeList<_1, TTypeList<_2, TTypeListTail<_3>>>>> final
{
  using Type = std::tuple<
    details::TItemConstIterator<std::remove_reference_t<_0>>, 
    details::TItemConstIterator<std::remove_reference_t<_1>>,
    details::TItemConstIterator<std::remove_reference_t<_2>>,
    details::TItemConstIterator<std::remove_reference_t<_3>>
  >;
};

} /// ::dy::expr namespace