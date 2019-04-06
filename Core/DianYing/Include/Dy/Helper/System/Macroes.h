#ifndef GUARD_DY_HELPER_MACROES_H
#define GUARD_DY_HELPER_MACROES_H
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

#include <Dy/Helper/System/Assertion.h>
#include <DyMacroPlatformSetting.h>

///
/// Do not touch below!
///

#if     defined(MDY_PLATFORM_FLAG_WINDOWS)
#define MDY_INHERITENCE_RESOURCE_PLATFORM_WINDOWS       public SDyRendererWindows
#define MDY_INHERITENCE_WINDOW_INFORMATION_SUPER MPlatformInfoWindows
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

/// DONT USE THIS DIRECTLY
#define MDY_TOKENPASTE(__MAX__, __MAY__) __MAX__ ## __MAY__
/// USE THIS DIRECTLY FOR CONCATNATION.
#define MDY_TOKENPASTE2(__MAX__, __MAY__) MDY_TOKENPASTE(__MAX__, __MAY__)

/// @macro MDY_MAKENAME
/// @breif Make name.
#define MDY_MAKENAME(__LHS__, __RHS__) MDY_TOKENPASTE2(__LHS__, __RHS__)

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
/// @macro MDY_CALL_BUT_NOUSE_RESULT
/// @brief Call expression but not use. result value.
///
#define MDY_CALL_BUT_NOUSE_RESULT(__MAExpression__) \
  { MDY_NOTUSED const auto MDY_TOKENPASTE2(_, __LINE__) = __MAExpression__; }

///
/// @macro MDY_NOUSE_RTVAL_EXPR
/// @brief Just neglect return value for calling function which return value is MDY_NODISCARD.
///
#define MDY_NOUSE_RTVAL_EXPR(__MAExpression__) \
  { MDY_NOTUSED const auto _ = __MAExpression__; }

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
    MDY_ASSERT_MSG(result == DY_SUCCESS, "Failed to execute expression successfully."); \
  }

/// 
/// @macro MDY_DELETE_RAWHEAP_SAFELY
/// @brief Delete raw (not unique_ptr and anything smart pointer) heap (not array) instance.
///
#define MDY_DELETE_RAWHEAP_SAFELY(__MAHeapInstance__) \
  { \
    delete __MAHeapInstance__; \
    __MAHeapInstance__ = MDY_INITIALIZE_NULL; \
  }

/// 
/// @macro MDY_DELETE_RAWHEAP_ARRAY_SAFELY
/// @brief Delete raw (not unique_ptr and anything smart pointer) heap array instance.
///
#define MDY_DELETE_RAWHEAP_ARRAY_SAFELY(__MAHeapInstance__) \
  { \
    delete[] __MAHeapInstance__; \
    __MAHeapInstance__ = MDY_INITIALIZE_NULL; \
  }

/// @macro MDY_INITIALIZE_DEFINT
/// @brief Initialize arbitary variable with -1.
#define MDY_INITIALIZE_DEFINT -1

/// @macro MDY_INITIALIZE_DEFUINT
/// @brief Initialize arbitary variable with 0.
#define MDY_INITIALIZE_DEFUINT 0

/// @macro MDY_NOT_INITIALIZED_STR
/// @brief Initialize arbitary string variable (const char*, std::string, std::string_view) with empty but '\0'.
#define MDY_INITIALIZE_EMPTYSTR ""

/// @macro MDY_INITIALIZE_NULL
/// @brief Initialize arbitary pointer with nullptr.
#define MDY_INITIALIZE_NULL nullptr

/// @macro MDY_BIND_BEGIN_END
/// @brief Help forward iteratable type to bind .begin() and .end() to function.
#define MDY_BIND_BEGIN_END(__MAIteratorableType__) __MAIteratorableType__.begin(), __MAIteratorableType__.end()

/// @macro MDY_BIND_CBEGIN_CEND
/// @brief Help forward iteratable type to bind .begin() and .end() to function.
#define MDY_BIND_CBEGIN_CEND(__MAIteratorableType__) __MAIteratorableType__.cbegin(), __MAIteratorableType__.cend()

/// @macro MDY_SET_IMMUTABLE_STRING
/// @brief Set immutable compile string_view variable __MAName__ with __MAString__.
#define MDY_SET_IMMUTABLE_STRING(__MAName__, __MAString__) \
  constexpr const char* __MAName__ = __MAString__

/// @macro MDY_UNEXEPCTED_BRANCH_BUT_RETURN
#define MDY_UNEXPECTED_BRANCH_BUT_RETURN(__MAExpression__) \
  MDY_UNEXPECTED_BRANCH(); \
  return (__MAExpression__)

/// @macro MDY_SLEEP_FOR_ATOMIC_TIME
/// @brief Sleep for atomic time (1 microsecond) for thread scheduling.
#define MDY_SLEEP_FOR_ATOMIC_TIME() \
{ \
  using namespace std::chrono_literals; \
  std::this_thread::sleep_for(0ms); \
}

/// @macro MDY_SYNC_LOCK_GUARD
/// @brief lock mutex reducing a amount of typing and verbosity.
#define MDY_SYNC_LOCK_GUARD(__MAMutex__) \
  std::lock_guard<decltype(__MAMutex__)> MDY_TOKENPASTE2(lock, __LINE__)(__MAMutex__)

/// @macro MDY_SYNC_WAIT_CONDITION
/// @brief lock mutex and wait condition is satisfied by reducing a amount of typing and verbosity.
#define MDY_SYNC_WAIT_CONDITION(__MAMutex__, __MAConditionVariable__, __MACondition__) \
  std::unique_lock<decltype(__MAMutex__)> lock(__MAMutex__); \
  __MAConditionVariable__.wait(lock, __MACondition__)

/// @macro MDY_SYNC_WAIT_CONDITION
/// @brief lock mutex and wait condition is satisfied by reducing a amount of typing and verbosity.
#define MDY_SYNC_WAIT_CONDITION_FOR(__MAMutex__, __MAConditionVariable__, __MATime__, __MACondition__) \
  std::unique_lock<decltype(__MAMutex__)> lock(__MAMutex__); \
  __MAConditionVariable__.wait_for(lock, __MATime__, __MACondition__)

/// @macro MDY_ATOMIC_COMPARE_SWAP_WEAK
/// @brief Do `compare and swap` to atomic variable weakly.
#define MDY_ATOMIC_COMPARE_SWAP_WEAK(__MAAtomic__, __MAExpected__, __MASwapValue__) \
  { \
    using __type = std::decay_t<decltype(__MAAtomic__)>; \
    __type::value_type __expect = __MAExpected__; \
    while (__MAAtomic__.compare_exchange_weak(__expect, __MASwapValue__) == false) \
      ; \
  }

/// @macro MDY_ATOMIC_COMPARE_SWAP_STRONG
/// @brief Do `compare and swap` to atomic variable strongly.
#define MDY_ATOMIC_COMPARE_SWAP_STRONG(__MAAtomic__, __MAExpected__, __MASwapValue__) \
  { \
    using __type = std::decay_t<decltype(__MAAtomic__)>; \
    __type::value_type __expect = __MAExpected__; \
    while (__MAAtomic__.compare_exchange_strong(__expect, __MASwapValue__) == false) \
      ; \
  }

/// @macro MDY_UNQMVCAST
/// @brief Static cast with moving of unique_ptr
#define MDY_UNQMVCAST(__MACastType__, __MAInstance__) \
  std::unqiue_ptr<__MACastType__*>(static_cast<__MACastType__*>(__MAInstance__.release()))

/// @macro MDY_TEST_PARAM_PACK_COUNT
/// @brief Check parameter pack count is same to speicified number.
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

/// @macro MDY_SET_CRC32_HASH
/// @brief Declare type member variable, which is used for storing hashed type value.
/// and value it in compile time.
#define MDY_SET_CRC32_HASH(__MAType__) \
public: \
static constexpr TU32 __mHashVal = dy::hash::ToHashCrc32(__MAType__);

/// @macro MDY_SET_CRC32_HASH
/// @brief Declare type member variable, which is used for storing hashed type value.
/// and value it in compile time.
#define MDY_SET_CRC32_HASH_WITH_TYPE(__MAType__) \
public: \
static constexpr TU32 __mHashVal = dy::hash::ToHashCrc32(MDY_TO_STRING(__MAType__));

/// @macro OVERRIDE_TYPEMATCH
/// @brief Define override function of component::CComponent's DoesTypeMatch() to match with
/// new type value of __DERIVED__ type. Defined function does not throw exception.
///
/// @param[in] __BASE__ Base type of __DERIVED__ type.
/// The argument has to be a full name, with namespace.
/// @param[in] __DERIVED__ __DERIVED__ type itself. The argument doesn't have to be a full name.
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

//!
//! Pointer to implementation section
//!

#define __DY__PIMPL Impl
#define __DY__PIMPL_DELETER __DY__PIMPL##Deleter

/// @def DY_DECLARE_PIMPL
/// @brief Declrare pimpl (pointer to implemetation) idiom into structure.
/// @reference https://stackoverflow.com/questions/9954518/stdunique-ptr-with-an-incomplete-type-wont-compile
#define DY_DECLARE_PIMPL() \
private: \
  struct __DY__PIMPL;\
  struct __DY__PIMPL_DELETER { void operator()(__DY__PIMPL*) const; }; \
  std::unique_ptr<__DY__PIMPL, __DY__PIMPL_DELETER> mImpl; \
public: 

/// @def DY_DEFINE_PIMPL
/// @brief Define on .cc (local translation unit). This macro must be on final translation unit file.
#define DY_DEFINE_PIMPL(__Type__) \
  void __Type__::__DY__PIMPL_DELETER::operator()(__Type__::__DY__PIMPL* ptr) const { delete ptr; }

/// @def DY_DEFINE_DEFAULT_DESTRUCTOR
/// @brief Define default destructor on .cc (local translation unit).
#define DY_DEFINE_DEFAULT_DESTRUCTOR(__Type__) __Type__::~__Type__() = default;

/// @def DY_DECLARE_PIMPL_CTOR
#define DY_DECLARE_PIMPL_CTOR(__Parent__) \
  __Parent__& mImplParent; \
  __DY__PIMPL(__Parent__& parent)
#define DY_DEFAULT_PIMPL_CTOR(__Parent__) \
  DY_DECLARE_PIMPL_CTOR(__Parent__) : mImplParent{parent} {};

/// @def DY_PIMPL
/// @brief Get pimpl instance.
#define DY_PIMPL this->mImpl

/// @def DY_INITIALIZE_PIMPL
/// @brief Initialize pimpl instance.
#define DY_INITIALIZE_PIMPL() \
  DY_PIMPL = std::unique_ptr<__DY__PIMPL, __DY__PIMPL_DELETER>(new __DY__PIMPL(*this))

/// @def DY_RESET_PIMPL
/// @brief Reset pimpl instance.
#define DY_RESET_PIMPL() DY_PIMPL = nullptr

//!
//! Others
//!

/// @macro MDY_INTERFACE
/// @brief INTERFACE because pure virtual function must be exposed to outside.
#define MDY_INTERFACE struct

/// @macro MDY_ABSTRACT
/// @brief ABSTRACT because non-static member variables might be in type.
#define MDY_ABSTRACT class

/// @macro MDY_GETSET
/// @brief Helping construction of get and set boilerplate function of member variable.
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

/// @macro _MIN_
/// @brief Specify that this parameter is input only.
#define _MIN_

/// @macro _MOUT_
/// @brief Specify that this parameter is output only.
#define _MOUT_

/// @macro _MIO_
/// @brief Specify that this paremeter will be read from function body and altered some properties.
/// @deprecated This macro will be deprecated.
#define _MIO_

/// @macro _MINOUT_
/// @brief Specify that this parameter will be read from function body and altered some properties.
#define _MINOUT_

#define MDY_ONLY_MOVEABLE_PROPERTIES_DEFAULT(__MAType__)    \
  __MAType__(const __MAType__&)                 = delete;   \
  __MAType__& operator=(const __MAType__&)      = delete;   \
  __MAType__(__MAType__&&)  noexcept            = default;  \
  __MAType__& operator=(__MAType__&&) noexcept  = default;

#define MDY_ONLY_MOVEABLE_PROPERTIES_CUSTOM(__MAType__) \
  __MAType__(const __MAType__&)             = delete;   \
  __MAType__& operator=(const __MAType__&)  = delete;   \
  __MAType__(__MAType__&&) noexcept;                    \
  __MAType__& operator=(__MAType__&&) noexcept;

#define MDY_NOT_COPYABLE_MOVEABLE_PROPERTIES(__MAType__)    \
  __MAType__(const __MAType__&)                 = delete;   \
  __MAType__& operator=(const __MAType__&)      = delete;   \
  __MAType__(__MAType__&&) noexcept             = delete;   \
  __MAType__& operator=(__MAType__&&) noexcept  = delete;

///
/// @macro  MDY_REFLECT_GETINSTANCE_AS
/// @brief  Get instance from reflection on compile time.
/// @param  __MAInstanceType__ Type for verifying.
/// @return Converted unique_ptr instance on runtime.
///
#define MDY_REFLECT_GETINSTANCE_AS(__MAInstanceType__) \
  ::dy::ConvertUniquePtrTo<__MAInstanceType__>(\
      ::dy::reflect::__Rfc__ObjectFactory::__Rfc__CreateInstance(#__MAInstanceType__) \
  )

///
/// @macro MDY_REFLECT_ENABLE
/// @brief Let reflection manager enable to apply reflection to given type.
/// @param __MAType__ Type for being enabled.
///
#define MDY_REFLECT_ENABLE(__MAType__) \
  private: \
  inline static ::dy::reflect::__Rfc__DerivedRegister<__MAType__> __rfc__Type{#__MAType__}

///
/// @macro MDY_REGISTER_RESOURCE
/// @brief Let reflection manager enable to apply reflection to given type.
/// @param __MAType__ Type for being enabled.
///
#define MDY_REGISTER_RESOURCE(__MAType__) \
  private: \
  inline static ::dy::reflect::RDyBuiltinResourceRegister<__MAType__> __rfc__register{#__MAType__}

///
/// @macro MDY_REGISTER_RESOURCE_WITH_SPECIFIER
/// @brief Let reflection manager enable to apply reflection to given type.
/// @param __MAType__ Type for being enabled.
/// @param __MAName__ Specification name to apply.
///
#define MDY_REGISTER_RESOURCE_WITH_SPECIFIER(__MAType__, __MAName__) \
  public: \
  inline static MDY_SET_IMMUTABLE_STRING(sName, __MAName__); \
  private: \
  inline static ::dy::reflect::RDyBuiltinResourceRegister<__MAType__> __rfc__register{sName};

///
/// @macro MDY_REGISTER_BOOT_RESOURCE_LIST
/// @brief Booting resource registering macro. Boot resource script must be only one.
///
#define MDY_REGISTER_BOOT_RESOURCE_LIST(__MASpecifier__) \
  MDY_REGISTER_RESOURCE(__MASpecifier__); \
  public: \
  __MASpecifier__();

///
/// @macro MDY_REGISTER_GLOBAL_RESOURCE_LIST
/// @brief Global resource registering macro. Global resource script can be multiple.
///
#define MDY_REGISTER_GLOBAL_RESOURCE_LIST(__MASpecifier__) \
  MDY_REGISTER_RESOURCE(__MASpecifier__); \
  public: \
  __MASpecifier__();

///
/// @macro MDY_LOADING_RESOURCE_BIND
/// @brief Bind resource requisition for loading resource.
///
#define MDY_LOADING_RESOURCE_BIND(__MAResourceType__, __MAString__) \
    this->mResourceRequisitionList.emplace_back(__MAResourceType__, __MAString__)

//!
//! Deserialize macro helper DDyVector, DDyVectorInt, DDyColor types.
//!

#define MDY_VECTOR_XY(__MAVectorType__)   __MAVectorType__.X, __MAVectorType__.Y
#define MDY_VECTOR_XYZ(__MAVectorType__)  __MAVectorType__.X, __MAVectorType__.Y, __MAVectorType__.Z
#define MDY_VECTOR_RGB(__MAVectorType__)  __MAVectorType__.R, __MAVectorType__.G, __MAVectorType__.B
#define MDY_VECTOR_RGBA(__MAVectorType__) __MAVectorType__.R, __MAVectorType__.G, __MAVectorType__.B, __MAVectorType__.A

///
/// @macro MDY_PRIVATE
/// @brief USE THIS JUST ONLY FOR HIDING PRIVATE FUNCTION OR VARIABLES EVEN FOR FRIEND CLASS AND FUNCTIONS.
///
#define MDY_PRIVATE(__MASpecifierName__) MDY_TOKENPASTE2(__, __MASpecifierName__)

//!
//! Function type macros.
//!

#define VARARGS     __cdecl										        /* Functions with variable arguments */
#ifndef CDECL
#define CDECL	    __cdecl											        /* Standard C function */
#endif
#define STDCALL		__stdcall										        /* Standard calling convention */
#define FORCEINLINE __forceinline							        /* Force code to be inline */
#define FORCENOINLINE __declspec(noinline)		        /* Force code to NOT be inline */
#define FUNCTION_NO_RETURN_START __declspec(noreturn)	/* Indicate that the function never returns. */
#define FUNCTION_NON_NULL_RETURN_START _Ret_notnull_	/* Indicate that the function never returns nullptr. */

#if defined(_WIN32)
#define MDY_NO_VTABLE __declspec(novtable)            /* NO virtual table only for pure abstract class. */
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
