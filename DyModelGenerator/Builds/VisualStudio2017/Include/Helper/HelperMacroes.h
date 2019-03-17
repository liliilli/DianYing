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

//!
//! Global macroes
//!

/// DONT USE THIS DIRECTLY
#define MDY_TOKENPASTE(__MAX__, __MAY__) __MAX__ ## __MAY__
/// USE THIS DIRECTLY FOR CONCATNATION.
#define MDY_TOKENPASTE2(__MAX__, __MAY__) MDY_TOKENPASTE(__MAX__, __MAY__)

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
/// @macro MDY_SINGLETON_PROPERTIES
/// This macro must not be attached to whichever class inherits Interface_Singleton<>.
///
#define MDY_SINGLETON_PROPERTIES(__MASingletonType__) \
public: \
    __MASingletonType__(const __MASingletonType__##&) = delete; \
    __MASingletonType__(__MASingletonType__##&&) = delete; \
    __MASingletonType__##& operator=(const __MASingletonType__##&) = delete; \
    __MASingletonType__##& operator=(__MASingletonType__##&&) = delete

///
/// @macro MDY_SINGLETON_DERIVED
/// This macro must not be attached to whichever class inherits Interface_Singleton<>.
///
#define MDY_SINGLETON_DERIVED(__MADerivedSingletonType__) \
private:                                                  \
    __MADerivedSingletonType__() = default;               \
    [[nodiscard]] EDySuccess pfInitialize();              \
    [[nodiscard]] EDySuccess pfRelease();                 \
    friend class Interface_Singleton<__MADerivedSingletonType__>;\
public:                                                   \
    virtual ~__MADerivedSingletonType__() = default;
