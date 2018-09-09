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
/// @macro
///
#define MDY_U8(__MAString__) u8##__MAString__

///
/// @macro
///
#define MDY_NOT_INITIALIZED_M1 -1

///
/// @macro
///
#define MDY_BIND_BEGIN_END(__MAIteratorableType__) \
  __MAIteratorableType__.begin(), __MAIteratorableType__.end()

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
/// @macro
///
#define MDY_SINGLETON_PROPERTIES(__MASingletonType__) \
public: \
    __MASingletonType__(const __MASingletonType__##&) = delete; \
    __MASingletonType__(__MASingletonType__##&&) = delete; \
    __MASingletonType__##& operator=(const __MASingletonType__##&) = delete; \
    __MASingletonType__##& operator=(__MASingletonType__##&&) = delete

///
/// @macro
///
#define MDY_SINGLETON_DERIVED(__MADerivedSingletonType__) \
private:                                                  \
    __MADerivedSingletonType__() = default;               \
    virtual ~__MADerivedSingletonType__() = default;      \
    [[nodiscard]] EDySuccess pfInitialize();              \
    [[nodiscard]] EDySuccess pfRelease();                 \
    friend class ISingleton<__MADerivedSingletonType__>

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
