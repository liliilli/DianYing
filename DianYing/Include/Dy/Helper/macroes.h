#ifndef GUARD_DY_HELPER_MACROES_H
#define GUARD_DY_HELPER_MACROES_H
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

#include <Dy/Helper/Assertion.h>

//!
//! Platform dependent macro
//!

#define MDY_PLATFORM_FLAG_WINDOWS
//#define MDY_PLATFORM_FLAG_LINUX
//#define MDY_PLATFORM_FLAG_MACOS

///
/// Do not touch below!
///

#if     defined(MDY_PLATFORM_FLAG_WINDOWS)
#define MDY_INHERITENCE_RESOURCE_PLATFORM_WINDOWS       public SDyRendererWindows
#define MDY_INHERITENCE_WINDOW_INFORMATION_SUPER public DDyWindowInformationWindows
#elif   defined(MDY_PLATFORM_FLAG_LINUX)
#define MDY_INHERITENCE_RESOURCE_PLATFORM_LINUX         public SDyRendererLinux
#define MDY_INHERITENCE_WINDOW_INFORMATION_SUPER public DDyWindowInformationLinux
#elif   defined(MDY_PLATFORM_FLAG_MACOS)
#define MDY_INHERITENCE_RESOURCE_PLATFORM_MACOS         public SDyRendererMacOs
#define MDY_INHERITENCE_WINDOW_INFORMATION_SUPER public DDyWindowInformationMacOs
#endif

//!
//! Global macroes
//!

///
/// @macro MDY_TO_STRING
/// @macro Convert __MAString__ to const char* literal.
///
#define MDY_TO_STRING(__MAString__) #__MAString__

///
/// @macro MDY_TRANSIENT
/// @brief TRANSIENT variable
///
#define MDY_TRANSIENT mutable

///
/// @macro MDY_NODISCARD
/// @brief Nodiscard specifier
///
#define MDY_NODISCARD [[nodiscard]]

///
/// @macro MDY_DEPRECATED
/// @brief Depereacted speicifer.
///
#define MDY_DEPRECATED(__MAVersion__, __MAFunction__, __MAAlternative__) \
  [[deprecated(MDY_TO_STRING(__MAFunction__) " is deprecated from " MDY_TO_STRING(__MAVersion__) ". Use " MDY_TO_STRING(__MAAlternative__) " instead.")]]

///
/// @macro MDY_FALLTHROUGH
/// @brief switch/case statement fallthrough next case.
///
#define MDY_FALLTHROUGH [[fallthrough]]

///
/// @macro MDY_NOTUSED
/// @brief Specified function or variable maybe unused. (to suppress warning /W4)
///
#define MDY_NOTUSED [[maybe_unused]]

///
/// @macro MDY_CHECK_ISNULL
/// @brief Check raw pointer is empty or not.
///
#define MDY_CHECK_ISNULL(__MAPointer__)     __MAPointer__ == nullptr

///
/// @macro MDY_CHECK_ISNULL
/// @brief Check raw pointer is empty or not.
///
#define MDY_CHECK_ISNOTNULL(__MAPointer__)  __MAPointer__ != nullptr

///
/// @macro MDY_CHECK_ISEMPTY
/// @brief Check smart pointer is empty.
///
#define MDY_CHECK_ISEMPTY(__MASmartPointer__)     __MASmartPointer__.get() == nullptr

///
/// @macro MDY_CHECK_ISNOTEMPTY
/// @brief Check smart pointer is not empty.
///
#define MDY_CHECK_ISNOTEMPTY(__MASmartPointer__)  __MASmartPointer__.get() != nullptr

///
/// @macro MDY_CASE_RETURN
/// @brief
///
#define MDY_CASE_RETURN(__Code__) case __Code__: return MDY_TO_STRING(__Code__)

///
/// @macro MDY_BITMASK_FLAG_TRUE
/// @brief
///
#define MDY_BITMASK_FLAG_TRUE(__MATarget__, __MAFlags__) __MATarget__ & __MAFlags__

///
/// @macro MDY_CHECK_EXECUTE
/// @brief
///
#define MDY_CHECK_EXECUTE(__MAFlag__, __MAFunctionCall__) \
  { \
    if constexpr (__MAFlag__) __MAFunctionCall__; \
  }

///
/// @macro MDY_CALL_ASSERT_SUCCESS
/// @brief Assert that expression must be successful or output error or message box when Windows.
///
#define MDY_CALL_ASSERT_SUCCESS(__MAFunctionCall__) \
  { \
    auto result = (__MAFunctionCall__); \
    result = result; \
    PHITOS_ASSERT(result == DY_SUCCESS, "Failed to execute expression successfully."); \
  }

///
/// @macro MDY_U8
/// @brief
///
#define MDY_U8(__MAString__) u8##__MAString__

///
/// @macro MDY_INITIALIZE_DEFINT
/// @brief Initialize arbitary variable with -1.
///
#define MDY_INITIALIZE_DEFINT -1

///
/// @macro MDY_INITIALIZE_DEFUINT
/// @brief Initialize arbitary variable with 0.
///
#define MDY_INITIALIZE_DEFUINT 0

///
/// @macro MDY_NOT_INITIALIZED_STR
/// @brief Initialize arbitary string variable (const char*, std::string, std::string_view) with empty but '\0'.
///
#define MDY_INITILAIZE_EMPTYSTR ""

///
/// @macro MDY_INITIALIZE_NULL
/// @brief Initialize arbitary pointer with nullptr.
///
#define MDY_INITIALIZE_NULL nullptr

///
/// @macro MSVSTR
/// @brief M(MDY) read SV(std::string_view) as STR(std::string or const char*) using .data()
///
#define MSVSTR(__MAStringView__) __MAStringView__.data()

///
/// @macro MDY_BIND_BEGIN_END
/// @brief Help forward iteratable type to bind .begin() and .end() to function.
///
#define MDY_BIND_BEGIN_END(__MAIteratorableType__) __MAIteratorableType__.begin(), __MAIteratorableType__.end()

///
/// @macro MDY_SET_IMMUTABLE_STRING
/// @brief Set immutable compile string_view variable __MAName__ with __MAString__.
///
#define MDY_SET_IMMUTABLE_STRING(__MAName__, __MAString__) \
  constexpr std::string_view __MAName__ = __MAString__

///
/// @macro MDY_UNQMVCAST
/// @brief Static cast with moving of unique_ptr
///
#define MDY_UNQMVCAST(__MACastType__, __MAInstance__) \
  std::unqiue_ptr<__MACastType__*>(static_cast<__MACastType__*>(__MAInstance__.release()))

///
/// @macro MDY_TEST_IS_BASE_OF
/// @brief Check __MADerivedType__ is derived from __MABaseType__.
///
#define MDY_TEST_IS_BASE_OF(__MABaseType__, __MADerivedType__) \
  static_assert(std::is_base_of_v<__MABaseType__, __MADerivedType__>, \
                MDY_TO_STRING(__MADerivedType__) " is not a derived type of " MDY_TO_STRING(__MABaseType__) ".")

///
/// @macro MDY_TEST_PARAM_PACK_COUNT
/// @brief Check parameter pack count is same to speicified number.
///
#define MDY_TEST_PARAM_PACK_COUNT(__MAParamPack__, __MAGoalCount__) \
  static_assert(sizeof...(__MAParamPack__) == __MAGoalCount__, \
                __FILE__ " " __LINE__ " Parameter pack test failed, the number of parameter pack must be " #__MAGoalCount__ ".")

//!
//! Do not touch below section!
//!

///
/// @macro Alternative version of DXTrace which removed since Win10 SDK.
/// This macro does not follow MDY_MACRO name convension because this macro
/// must substitute original HR WIN32 SDK macro.
///
/// @reference http://wergia.tistory.com/6
///
#if defined(_WIN32)
  #if !defined(NDEBUG)
    #ifndef HR
      #define HR(__MAExpression__) \
      { \
        HRESULT err = (__MAExpression__); \
        if (err != S_OK) { \
          LPWSTR errorOutputString; \
          FormatMessage( \
              FORMAT_MESSAGE_FROM_SYSTEM | \
              FORMAT_MESSAGE_IGNORE_INSERTS | \
              FORMAT_MESSAGE_ALLOCATE_BUFFER, \
              nullptr, \
              err, \
              MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), \
              (LPTSTR)&errorOutputString, \
              0, \
              nullptr); \
          MessageBox(nullptr, errorOutputString, L"Error", MB_OK); \
        } \
      }
    #endif
  #else
    #ifndef HR
      #define HR(__MAExpression__) (__MAExpression__)
    #endif
  #endif
#endif

///
/// @macro
///
#if defined(NDEBUG)
#define MDY_VULKAN_FLAG_VALIDATION_SET false
#else
#define MDY_VULKAN_FLAG_VALIDATION_SET true
#endif

//!
//! Macroes related to implementation
//!

///
/// @macro MDY_SET_CRC32_HASH
/// @brief Declare type member variable, which is used for storing hashed type value.
/// and value it in compile time.
///
#define MDY_SET_CRC32_HASH(__MAType__) \
public: \
static constexpr TU32 __mHashVal = dy::hash::DyToCrc32Hash(__MAType__);

///
/// @macro MDY_SET_CRC32_HASH
/// @brief Declare type member variable, which is used for storing hashed type value.
/// and value it in compile time.
///
#define MDY_SET_CRC32_HASH_WITH_TYPE(__MAType__) \
public: \
static constexpr TU32 __mHashVal = dy::hash::DyToCrc32Hash(MDY_TO_STRING(__MAType__));

///
/// @macro OVERRIDE_TYPEMATCH
/// @brief Define override function of component::CComponent's DoesTypeMatch() to match with
/// new type value of __DERIVED__ type. Defined function does not throw exception.
///
/// @param[in] __BASE__ Base type of __DERIVED__ type.
/// The argument has to be a full name, with namespace.
/// @param[in] __DERIVED__ __DERIVED__ type itself. The argument doesn't have to be a full name.
///
#define MDY_SET_TYPEMATCH_FUNCTION(__BASE__, __DERIVED__) \
public: \
virtual bool IsTypeMatched(const TU32 hashVal) const noexcept override { \
  if (__DERIVED__::__mHashVal == hashVal) \
  { \
    return true; \
  } \
  else \
  { \
    return __BASE__::IsTypeMatched(hashVal); \
  } \
}

///
/// @macro MDY_INTERFACE
/// @brief INTERFACE because pure virtual function must be exposed to outside.
///
#define MDY_INTERFACE struct

///
/// @macro MDY_ABSTRACT
/// @brief ABSTRACT because non-static member variables might be in type.
///
#define MDY_ABSTRACT class

///
/// @macro MDY_GETSET
/// @brief Helping construction of get and set boilerplate function of member variable.
///
#define MDY_GETSET(__MAVariable__)                                                    \
  [[nodiscard]] const decltype(__MAVariable__)& Get##__MAVariable__() const noexcept  \
  {                                                                                   \
    return this->__MAVariable__;                                                      \
  }                                                                                   \
                                                                                      \
  void Set##__MAVariable__(const decltype(__MAVariable__)& input##__MAVariable__)     \
  {                                                                                   \
    this->__MAVariable__ = input##__MAVariable__;                                     \
  }

///
/// @macro _MIN_
/// @brief Specify that this parameter is input only.
///
#define _MIN_

///
/// @macro _MOUT_
/// @brief Specify that this parameter is output only.
///
#define _MOUT_

///
/// @macro _MIO_
/// @brief Specify that this paremeter will be read from function body and altered some properties.
///
#define _MIO_

///
/// @macro MDY_INTERFACE_PROPERTY
/// @brief INTERFACE variable
/// @todo NOT WORKING! FIX THIS SOMEDAY.
///
#define MDY_INTERFACE_PROPERTY(__MAInterfaceType__) \
  static_assert(std::is_abstract_v<__MAInterfaceType__>, MDY_TO_STRING(__MAInterfaceType__) " is not satisfied interface property.")

///
/// @macro MDY_SINGLETON_PROPERTIES
/// This macro must not be attached to whichever class inherits ISingleton<>.
///
#define MDY_SINGLETON_PROPERTIES(__MASingletonType__) \
public: \
    __MASingletonType__(const __MASingletonType__##&) = delete; \
    __MASingletonType__(__MASingletonType__##&&) = delete; \
    __MASingletonType__##& operator=(const __MASingletonType__##&) = delete; \
    __MASingletonType__##& operator=(__MASingletonType__##&&) = delete

///
/// @macro MDY_SINGLETON_DERIVED
/// This macro must not be attached to whichever class inherits ISingleton<>.
///
#define MDY_SINGLETON_DERIVED(__MADerivedSingletonType__) \
private:                                                  \
    __MADerivedSingletonType__() = default;               \
    virtual ~__MADerivedSingletonType__() = default;      \
    [[nodiscard]] EDySuccess pfInitialize();              \
    [[nodiscard]] EDySuccess pfRelease();                 \
    friend class ISingleton<__MADerivedSingletonType__>

#if defined(MDY_FLAG_IN_EDITOR)
///
/// @macro MDY_GUISINGLETON_PROPERTIES
/// @brief Set properties of gui window singleton types.
/// This macro must not be attached to whichever class inherits IDyGuiWindowSingleton<>.
///
#define MDY_GUISINGLETON_PROPERTIES(__MASingletonType__) \
public: \
    __MASingletonType__(const __MASingletonType__##&) = delete; \
    __MASingletonType__(__MASingletonType__##&&) = delete; \
    __MASingletonType__##& operator=(const __MASingletonType__##&) = delete; \
    __MASingletonType__##& operator=(__MASingletonType__##&&) = delete

///
/// @macro MDY_GUISINGLETON_DERIVED
/// @brief Set boilerplate functions for gui window singleton types.
/// This macro must not be attached to whichever class inherits IDyGuiWindowSingleton<>.
///
#define MDY_GUISINGLETON_DERIVED(__MADerivedSingletonType__, __MAConstructionDescriptorType__)  \
public:                                                   \
    __MADerivedSingletonType__() = default;               \
    virtual ~__MADerivedSingletonType__() = default;      \
private:                                                  \
    [[nodiscard]] EDySuccess pfInitialize([[maybe_unused]] const __MAConstructionDescriptorType__& desc); \
    [[nodiscard]] EDySuccess pfRelease();                 \
    MDY_SET_CRC32_HASH_WITH_TYPE(__MADerivedSingletonType__); \
    MDY_SET_TYPEMATCH_FUNCTION(IDyGuiComponentBase, __MADerivedSingletonType__); \
    friend class IDyGuiWinSingleton<__MADerivedSingletonType__, __MAConstructionDescriptorType__>
#endif /// MDY_FLAG_IN_EDITOR

#define MDY_ONLY_MOVEABLE_PROPERTIES_DEFAULT(__MAType__)\
  __MAType__(const __MAType__&)             = delete;   \
  __MAType__& operator=(const __MAType__&)  = delete;   \
  __MAType__(__MAType__&&)                  = default;  \
  __MAType__& operator=(__MAType__&&)       = default;  \

#define MDY_ONLY_MOVEABLE_PROPERTIES_CUSTOM(__MAType__) \
  __MAType__(const __MAType__&)             = delete;   \
  __MAType__& operator=(const __MAType__&)  = delete;   \
  __MAType__(__MAType__&&);                             \
  __MAType__& operator=(__MAType__&&);                  \

#define MDY_NOT_COPYABLE_MOVEABLE_PROPERTIES(__MAType__)\
  __MAType__(const __MAType__&)             = delete;   \
  __MAType__& operator=(const __MAType__&)  = delete;   \
  __MAType__(__MAType__&&)                  = delete;   \
  __MAType__& operator=(__MAType__&&)       = delete;   \

//!
//! Function type macros.
//!

#define VARARGS     __cdecl										        // Functions with variable arguments */
#ifndef CDECL
#define CDECL	    __cdecl											        // Standard C function */
#endif
#define STDCALL		__stdcall										        // Standard calling convention */
#define FORCEINLINE __forceinline							        // Force code to be inline */
#define FORCENOINLINE __declspec(noinline)		        // Force code to NOT be inline */
#define FUNCTION_NO_RETURN_START __declspec(noreturn)	// Indicate that the function never returns. */
#define FUNCTION_NON_NULL_RETURN_START _Ret_notnull_	// Indicate that the function never returns nullptr. */

#if defined(_WIN32)
#define MDY_NO_VTABLE __declspec(novtable)            // NO virtual table only for pure abstract class.
#else
#define MDY_NO_VTABLE
#endif

///
/// Macro static integrity test
///

#if defined(MDY_PLATFORM_FLAG_WINDOWS)
#if defined(MDY_PLATFORM_FLAG_LINUX) || defined(MDY_PLATFORM_FLAG_MACOS)
static_assert(false, "Platform flag multi checking is prohibited.");
#endif
#elif defined(MDY_PLATFORM_FLAG_LINUX)
#if defined(MDY_PLATFORM_FLAG_WINDOWS) || defined(MDY_PLATFORM_FLAG_MACOS)
static_assert(false, "Platform flag multi checking is prohibited.");
#else
static_assert(false, "Linux platform is not supported now.");
#endif
#elif defined(MDY_PLATFORM_FLAG_MACOS)
#if defined(MDY_PLATFORM_FLAG_LINUX) || defined(MDY_PLATFORM_FLAG_WINDOWS)
static_assert(false, "Platform flag multi checking is prohibited.");
#else
static_assert(false, "Windows platform is not supported now.");
#endif
#else
static_assert(false, "Must set up one platform build option.");
#endif


#endif /// GUARD_DY_HELPER_MACROES_H
