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

#include <Expr/XMacroes.h>
#include <Expr/TEnumString.h>

namespace dy::expr
{

#define __EXPR_ENUMTYPE_TODETAIL_TMPL_true(__EnumType__)  template <__EnumType__ E>
#define __EXPR_ENUMTYPE_TODETAIL_TMPL_false(__EnumType__) template <EXPR_E(__EnumType__) E>

#define __EXPR_ENUMTYPE_TODETAIL_DECR_true(__EnumType__) \
  __EXPR_ENUMTYPE_TODETAIL_TMPL_true(__EnumType__) struct __XToDetailType;

#define __EXPR_ENUMTYPE_TODETAIL_DECR_false(__EnumType__) \
  __EXPR_ENUMTYPE_TODETAIL_TMPL_false(__EnumType__) struct __XToDetailType;

#define __EXPR_ENUMTYPE_INTENRAL_SPECIALIZATION(__EnumType__, __IsPlainEnumType__, __Type__, __Enum__) \
  template <> struct EXPR_MAKENAME(__XTo, __EnumType__)<__Type__> { static constexpr auto value = __EnumType__::__Enum__; }; \
  template <> struct __XToDetailType<__EnumType__::__Enum__> { using Type = __Type__; }

/// @brief Special case.
#define __EXPR_ENUMTYPE_INTERNAL_0(__EnumType__, __IsPlainEnumType__)

//!
//! macro folding expression.
//! 

#define __EXPR_ENUMTYPE_INTERNAL_2(__EnumType__, __IsPlainEnumType__, __Type__, __Enum__, ...) \
  __EXPR_ENUMTYPE_INTENRAL_SPECIALIZATION(__EnumType__, __IsPlainEnumType__, __Type__, __Enum__);

#define __EXPR_ENUMTYPE_INTERNAL_4(__EnumType__, __IsPlainEnumType__, __Type__, __Enum__, ...) \
  __EXPR_ENUMTYPE_INTENRAL_SPECIALIZATION(__EnumType__, __IsPlainEnumType__, __Type__, __Enum__); \
  EXPAND(__EXPR_ENUMTYPE_INTERNAL_2(__EnumType__, __IsPlainEnumType__, __VA_ARGS__));

#define __EXPR_ENUMTYPE_INTERNAL_6(__EnumType__, __IsPlainEnumType__, __Type__, __Enum__, ...) \
  __EXPR_ENUMTYPE_INTENRAL_SPECIALIZATION(__EnumType__, __IsPlainEnumType__, __Type__, __Enum__); \
  EXPAND(__EXPR_ENUMTYPE_INTERNAL_4(__EnumType__, __IsPlainEnumType__, __VA_ARGS__));

#define __EXPR_ENUMTYPE_INTERNAL_8(__EnumType__, __IsPlainEnumType__, __Type__, __Enum__, ...) \
  __EXPR_ENUMTYPE_INTENRAL_SPECIALIZATION(__EnumType__, __IsPlainEnumType__, __Type__, __Enum__); \
  EXPAND(__EXPR_ENUMTYPE_INTERNAL_6(__EnumType__, __IsPlainEnumType__, __VA_ARGS__));

#define __EXPR_ENUMTYPE_INTERNAL_10(__EnumType__, __IsPlainEnumType__, __Type__, __Enum__, ...) \
  __EXPR_ENUMTYPE_INTENRAL_SPECIALIZATION(__EnumType__, __IsPlainEnumType__, __Type__, __Enum__); \
  EXPAND(__EXPR_ENUMTYPE_INTERNAL_8(__EnumType__, __IsPlainEnumType__, __VA_ARGS__));

#define __EXPR_ENUMTYPE_INTERNAL_12(__EnumType__, __IsPlainEnumType__, __Type__, __Enum__, ...) \
  __EXPR_ENUMTYPE_INTENRAL_SPECIALIZATION(__EnumType__, __IsPlainEnumType__, __Type__, __Enum__); \
  EXPAND(__EXPR_ENUMTYPE_INTERNAL_10(__EnumType__, __IsPlainEnumType__, __VA_ARGS__));

#define __EXPR_ENUMTYPE_INTERNAL_14(__EnumType__, __IsPlainEnumType__, __Type__, __Enum__, ...) \
  __EXPR_ENUMTYPE_INTENRAL_SPECIALIZATION(__EnumType__, __IsPlainEnumType__, __Type__, __Enum__); \
  EXPAND(__EXPR_ENUMTYPE_INTERNAL_12(__EnumType__, __IsPlainEnumType__, __VA_ARGS__));

#define __EXPR_ENUMTYPE_INTERNAL_16(__EnumType__, __IsPlainEnumType__, __Type__, __Enum__, ...) \
  __EXPR_ENUMTYPE_INTENRAL_SPECIALIZATION(__EnumType__, __IsPlainEnumType__, __Type__, __Enum__); \
  EXPAND(__EXPR_ENUMTYPE_INTERNAL_14(__EnumType__, __IsPlainEnumType__, __VA_ARGS__));

#define __EXPR_ENUMTYPE_INTERNAL_18(__EnumType__, __IsPlainEnumType__, __Type__, __Enum__, ...) \
  __EXPR_ENUMTYPE_INTENRAL_SPECIALIZATION(__EnumType__, __IsPlainEnumType__, __Type__, __Enum__); \
  EXPAND(__EXPR_ENUMTYPE_INTERNAL_16(__EnumType__, __IsPlainEnumType__, __VA_ARGS__));

#define __EXPR_ENUMTYPE_INTERNAL_20(__EnumType__, __IsPlainEnumType__, __Type__, __Enum__, ...) \
  __EXPR_ENUMTYPE_INTENRAL_SPECIALIZATION(__EnumType__, __IsPlainEnumType__, __Type__, __Enum__); \
  EXPAND(__EXPR_ENUMTYPE_INTERNAL_18(__EnumType__, __IsPlainEnumType__, __VA_ARGS__));

#define __EXPR_ENUMTYPE_INTERNAL_22(__EnumType__, __IsPlainEnumType__, __Type__, __Enum__, ...) \
  __EXPR_ENUMTYPE_INTENRAL_SPECIALIZATION(__EnumType__, __IsPlainEnumType__, __Type__, __Enum__); \
  EXPAND(__EXPR_ENUMTYPE_INTERNAL_20(__EnumType__, __IsPlainEnumType__, __VA_ARGS__));

#define __EXPR_ENUMTYPE_INTERNAL_24(__EnumType__, __IsPlainEnumType__, __Type__, __Enum__, ...) \
  __EXPR_ENUMTYPE_INTENRAL_SPECIALIZATION(__EnumType__, __IsPlainEnumType__, __Type__, __Enum__); \
  EXPAND(__EXPR_ENUMTYPE_INTERNAL_22(__EnumType__, __IsPlainEnumType__, __VA_ARGS__));

#define __EXPR_ENUMTYPE_INTERNAL_26(__EnumType__, __IsPlainEnumType__, __Type__, __Enum__, ...) \
  __EXPR_ENUMTYPE_INTENRAL_SPECIALIZATION(__EnumType__, __IsPlainEnumType__, __Type__, __Enum__); \
  EXPAND(__EXPR_ENUMTYPE_INTERNAL_24(__EnumType__, __IsPlainEnumType__, __VA_ARGS__));

#define __EXPR_ENUMTYPE_INTERNAL_28(__EnumType__, __IsPlainEnumType__, __Type__, __Enum__, ...) \
  __EXPR_ENUMTYPE_INTENRAL_SPECIALIZATION(__EnumType__, __IsPlainEnumType__, __Type__, __Enum__); \
  EXPAND(__EXPR_ENUMTYPE_INTERNAL_26(__EnumType__, __IsPlainEnumType__, __VA_ARGS__));

#define __EXPR_ENUMTYPE_INTERNAL_30(__EnumType__, __IsPlainEnumType__, __Type__, __Enum__, ...) \
  __EXPR_ENUMTYPE_INTENRAL_SPECIALIZATION(__EnumType__, __IsPlainEnumType__, __Type__, __Enum__); \
  EXPAND(__EXPR_ENUMTYPE_INTERNAL_28(__EnumType__, __IsPlainEnumType__, __VA_ARGS__));

#define __EXPR_ENUMTYPE_INTERNAL_32(__EnumType__, __IsPlainEnumType__, __Type__, __Enum__, ...) \
  __EXPR_ENUMTYPE_INTENRAL_SPECIALIZATION(__EnumType__, __IsPlainEnumType__, __Type__, __Enum__); \
  EXPAND(__EXPR_ENUMTYPE_INTERNAL_30(__EnumType__, __IsPlainEnumType__, __VA_ARGS__));

#define __EXPR_ENUMTYPE_INTERNAL_34(__EnumType__, __IsPlainEnumType__, __Type__, __Enum__, ...) \
  __EXPR_ENUMTYPE_INTENRAL_SPECIALIZATION(__EnumType__, __IsPlainEnumType__, __Type__, __Enum__); \
  EXPAND(__EXPR_ENUMTYPE_INTERNAL_32(__EnumType__, __IsPlainEnumType__, __VA_ARGS__));

#define __EXPR_ENUMTYPE_INTERNAL_36(__EnumType__, __IsPlainEnumType__, __Type__, __Enum__, ...) \
  __EXPR_ENUMTYPE_INTENRAL_SPECIALIZATION(__EnumType__, __IsPlainEnumType__, __Type__, __Enum__); \
  EXPAND(__EXPR_ENUMTYPE_INTERNAL_34(__EnumType__, __IsPlainEnumType__, __VA_ARGS__));

#define __EXPR_ENUMTYPE_INTERNAL_38(__EnumType__, __IsPlainEnumType__, __Type__, __Enum__, ...) \
  __EXPR_ENUMTYPE_INTENRAL_SPECIALIZATION(__EnumType__, __IsPlainEnumType__, __Type__, __Enum__); \
  EXPAND(__EXPR_ENUMTYPE_INTERNAL_36(__EnumType__, __IsPlainEnumType__, __VA_ARGS__));

#define __EXPR_ENUMTYPE_INTERNAL_40(__EnumType__, __IsPlainEnumType__, __Type__, __Enum__, ...) \
  __EXPR_ENUMTYPE_INTENRAL_SPECIALIZATION(__EnumType__, __IsPlainEnumType__, __Type__, __Enum__); \
  EXPAND(__EXPR_ENUMTYPE_INTERNAL_38(__EnumType__, __IsPlainEnumType__, __VA_ARGS__));

#define __EXPR_ENUMTYPE_INTERNAL_42(__EnumType__, __IsPlainEnumType__, __Type__, __Enum__, ...) \
  __EXPR_ENUMTYPE_INTENRAL_SPECIALIZATION(__EnumType__, __IsPlainEnumType__, __Type__, __Enum__); \
  EXPAND(__EXPR_ENUMTYPE_INTERNAL_40(__EnumType__, __IsPlainEnumType__, __VA_ARGS__));

#define __EXPR_ENUMTYPE_INTERNAL_44(__EnumType__, __IsPlainEnumType__, __Type__, __Enum__, ...) \
  __EXPR_ENUMTYPE_INTENRAL_SPECIALIZATION(__EnumType__, __IsPlainEnumType__, __Type__, __Enum__); \
  EXPAND(__EXPR_ENUMTYPE_INTERNAL_42(__EnumType__, __IsPlainEnumType__, __VA_ARGS__));

#define __EXPR_ENUMTYPE_INTERNAL_46(__EnumType__, __IsPlainEnumType__, __Type__, __Enum__, ...) \
  __EXPR_ENUMTYPE_INTENRAL_SPECIALIZATION(__EnumType__, __IsPlainEnumType__, __Type__, __Enum__); \
  EXPAND(__EXPR_ENUMTYPE_INTERNAL_44(__EnumType__, __IsPlainEnumType__, __VA_ARGS__));

#define __EXPR_ENUMTYPE_INTERNAL_48(__EnumType__, __IsPlainEnumType__, __Type__, __Enum__, ...) \
  __EXPR_ENUMTYPE_INTENRAL_SPECIALIZATION(__EnumType__, __IsPlainEnumType__, __Type__, __Enum__); \
  EXPAND(__EXPR_ENUMTYPE_INTERNAL_46(__EnumType__, __IsPlainEnumType__, __VA_ARGS__));

//!
//! etc and Entry api
//! 

#define __EXPR_ENUMTYPE_SPECIALIZATION_ENTRY(__EnumType__, __IsPlainEnumType__, ...) \
  EXPAND(EXPR_MAKENAME(__EXPR_ENUMTYPE_INTERNAL_, EXPR_GET_COUNT_ARGS(__VA_ARGS__))(__EnumType__, __IsPlainEnumType__, __VA_ARGS__))

/// @def EXPR_DEFINE_ENUMTYPE_BINDING
/// @brief Create code for conversion with type paramter between each a pair of type and enumration type.
/// If Enumation type is not plain type but created with EXPR_DEFINE_ENUM, __IsPlainEnumType__ must be true.
/// @example
///
/// EXPR_DEFINE_ENUMTYPE_BINDING(Enumration type name, false | true, 
///   Type_0, Enum_A,
///   Type_1, Enum_B,
///   ...);
#define EXPR_DEFINE_ENUMTYPE_BINDING(__EnumType__, __IsPlainEnumType__, ...) \
  /* Declaration */ \
  template <typename T> struct EXPR_MAKENAME(__XTo, __EnumType__); \
  EXPR_MAKENAME(__EXPR_ENUMTYPE_TODETAIL_DECR_, __IsPlainEnumType__)(__EnumType__); \
  \
  /* Helper API */ \
  template <typename T> \
  static constexpr auto EXPR_MAKENAME(To, __EnumType__) = EXPR_MAKENAME(__XTo, __EnumType__)<T>::value; \
  \
  EXPR_MAKENAME(__EXPR_ENUMTYPE_TODETAIL_TMPL_, __IsPlainEnumType__)(__EnumType__) \
  using ToDetailType = typename __XToDetailType<E>::Type; \
  \
  /* Implementation */ \
  EXPAND(__EXPR_ENUMTYPE_SPECIALIZATION_ENTRY(__EnumType__, __IsPlainEnumType__, __VA_ARGS__))
  
} /// ::dy::expr namespace
