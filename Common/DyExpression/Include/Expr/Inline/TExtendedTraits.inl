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

#include <type_traits>

namespace dy::expr::details
{

// To allow ADL with custom begin/end
using std::begin;
using std::end;

struct IsIterable_T final
{

template <typename T>
static auto func(int)
  -> decltype (
    begin(std::declval<T&>()) != end(std::declval<T&>()),   // begin/end and operator !=
    void(), // Handle evil operator ,
    ++std::declval<decltype(begin(std::declval<T&>()))&>(), // operator ++
    void(*begin(std::declval<T&>())), // operator*
    std::true_type{}
  );

template <typename T>
static std::false_type func(...);

};

struct HasBeginEnd_T final
{

template <typename T>
static auto func(int)
  -> decltype(
    begin(std::declval<T&>()) != end(std::declval<T&>()),   // Begin / End and operator !=
    std::true_type{}
  );

template <typename T>
static std::false_type func(...);

};

} /// ::dy::expr::details namespace
