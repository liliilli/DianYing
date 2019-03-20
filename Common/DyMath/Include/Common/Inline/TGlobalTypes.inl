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

enum class EValueCategory
{
  Character = 0,
  Integer = 1,
  Real = 2
};

template <typename TType> struct TValueCategory;

#define MATH_VALUE_CATEGORY(TType, Category) \
  template <> struct TValueCategory<TType> \
  { \
    using Type = TType; \
    static constexpr EValueCategory kValue = EValueCategory::Category; \
    static constexpr size_t kTypeSize = sizeof(Type); \
  };

MATH_VALUE_CATEGORY(TWChr, Character)
MATH_VALUE_CATEGORY(TChr8, Character)
MATH_VALUE_CATEGORY(TChr16, Character)
MATH_VALUE_CATEGORY(TChr32, Character)

MATH_VALUE_CATEGORY(TI8, Integer)
MATH_VALUE_CATEGORY(TI16, Integer)
MATH_VALUE_CATEGORY(TI32, Integer)
MATH_VALUE_CATEGORY(TI64, Integer)

MATH_VALUE_CATEGORY(TU8, Integer)
MATH_VALUE_CATEGORY(TU16, Integer)
MATH_VALUE_CATEGORY(TU32, Integer)
MATH_VALUE_CATEGORY(TU64, Integer)

MATH_VALUE_CATEGORY(TF32, Real)
MATH_VALUE_CATEGORY(TF64, Real)

template <typename TType>
constexpr EValueCategory kCategoryOf = TValueCategory<TType>::kValue;

template <typename TType>
constexpr size_t kTypeSizeOf = TValueCategory<TType>::kTypeSize;

} /// ::dy::math namespace