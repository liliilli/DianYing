#ifndef GUARD_DY_CORE_REFLECTION_RDYBUILTINRESOURCE_H
#define GUARD_DY_CORE_REFLECTION_RDYBUILTINRESOURCE_H
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

#include <unordered_map>
#include <utility>
#include <memory>

namespace dy::reflect
{

///
/// @brief
/// @return
///
template<typename TType> void __Rfc__CreateBuiltin() { TType value{}; }

///
/// @struct __Rfc__BuiltinResourceMetaList
/// @brief
///
class __Rfc__BuiltinResourceMetaList
{
public:
  using TValueType  = void(*)();
  using TMapType    = std::unordered_map<std::string, TValueType>;

  ///
  /// @brief
  /// @param  typeSpecifier
  /// @return
  ///
  static void __Rfc__UploadBuiltinResources()
  {
    auto& map = __Rfc__GetTypeInitMapReference();
    for (const auto& [specifier, func] : map) { func(); }
  }

protected:
  ///
  /// @brief
  /// @return
  ///
  static MDY_NODISCARD TMapType& __Rfc__GetTypeInitMapReference()
  {
    static TMapType typeMap;
    return typeMap;
  }
};

///
/// @struct __Rfc__BtResourceRegister
/// @brief
///
template <typename TType>
struct __Rfc__BtResourceRegister final : public __Rfc__BuiltinResourceMetaList
{
  __Rfc__BtResourceRegister(_MIN_ const std::string& typeSpecifier)
  {
    __Rfc__GetTypeInitMapReference().try_emplace(typeSpecifier, &__Rfc__CreateBuiltin<TType>);
  }
};

} /// ::dy::reflect namespace

///
/// @macro MDY_REFLECT_ENABLE_RESOURCE
/// @brief Let reflection manager enable to apply reflection to given type.
/// @param __MAType__ Type for being enabled.
///
#define MDY_REFLECT_ENABLE_RESOURCE(__MAType__) \
  private: \
  inline static ::dy::reflect::__Rfc__BtResourceRegister<__MAType__> __rfc__Type{#__MAType__}

#define MDY_REFLECT_SETUP_BUILTIN_RESOURCE() \
  ::dy::reflect::__Rfc__BuiltinResourceMetaList::__Rfc__UploadBuiltinResources()

#endif /// GUARD_DY_CORE_REFLECTION_RDYBUILTINRESOURCE_H