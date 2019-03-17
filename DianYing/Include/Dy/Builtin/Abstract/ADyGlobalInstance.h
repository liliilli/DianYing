#ifndef GUARD_DY_BUILTIN_ABSTRACT_ADyGlobalInstance_H
#define GUARD_DY_BUILTIN_ABSTRACT_ADyGlobalInstance_H
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

namespace dy
{

//!
//! HOW-TO-USE
//! 1. Make new type, inheritencing `ADyGlobalInstance`.
//! 2. Set Macro `MDY_GLOBALINSTANCE_REGISTER` with new type name on above of new global instance type.
//! 3. Set Macro `MDY_GLOBALINSTANCE_PROPERTY` as public in global instance type declaration body.
//! 4. Done!
//!
  
/// @macro MDY_GLOBALINSTANCE_REGISTER
/// @brief Register global instance type to Dy system.
#define MDY_GLOBALINSTANCE_REGISTER(__MAGlobalInstance__) \
class __MAGlobalInstance__; \
template <> \
class ::dy::TDyGlobalInstance<::dy::hash::DyToCrc32Hash(MDY_TO_STRING(__MAGlobalInstance__))> \
{ \
public: \
  inline static constexpr auto kHashVal = hash::DyToCrc32Hash(MDY_TO_STRING(__MAGlobalInstance__)); \
  using Type = __MAGlobalInstance__; \
}; 

/// @macro MDY_GLOBALINSTANCE_PROPERTY
/// @brief Set essential properties of given Global instance type.
#define MDY_GLOBALINSTANCE_PROPERTY(__MAGlobalInstance__) \
inline static constexpr auto kHashVal = ::dy::TDyGlobalInstance<hash::DyToCrc32Hash(MDY_TO_STRING(__MAGlobalInstance__))>::kHashVal; \
inline static ::dy::reflect::RGlobalInstanceRegistration<__MAGlobalInstance__> __rfc__Register{kHashVal};

/// @class ADyGlobalInstance
/// @brief Dy global instance base type that presist until end of application.
class ADyGlobalInstance { };

} /// ::dy namespace

#endif /// GUARD_DY_BUILTIN_ABSTRACT_ADyGlobalInstance_H