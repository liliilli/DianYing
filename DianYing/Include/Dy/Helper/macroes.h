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
/// @macro MDY_TRANSIENT
/// @brief TRANSIENT variable
///
#define MDY_TRANSIENT mutable

///
/// @macro
///
#define MDY_TO_STRING(__MAString__) #__MAString__

///
/// @macro
///
#define MDY_CASE_RETURN(__Code__) case __Code__: return MDY_TO_STRING(__Code__)

///
/// @macro
///
#define MDY_BITMASK_FLAG_TRUE(__MATarget__, __MAFlags__) __MATarget__ & __MAFlags__

///
/// @macro
///
#define MDY_CHECK_EXECUTE(__MAFlag__, __MAFunctionCall__) \
  { \
    if constexpr (__MAFlag__) __MAFunctionCall__; \
  }

///
/// @macro
///
#define MDY_CALL_ASSERT_SUCCESS(__MAFunctionCall__) \
  { \
    auto result = (__MAFunctionCall__); \
    result = result; \
    assert(result == DY_SUCCESS); \
  }

///
/// @macro MDY_U8
///
#define MDY_U8(__MAString__) u8##__MAString__

///
/// @macro MDY_NOT_INITIALIZED_M1
/// @brief Initialize arbitary variable with -1.
///
#define MDY_NOT_INITIALIZED_M1 -1

///
/// @macro MDY_NOT_INITIALIZED_0
/// @brief Initialize arbitary variable with 0.
///
#define MDY_NOT_INITIALIZED_0   0

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
