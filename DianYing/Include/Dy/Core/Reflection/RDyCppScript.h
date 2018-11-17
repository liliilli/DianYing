#ifndef MDY_CORE_REFLECTION_RDYCPPSCRIPT_H
#define MDY_CORE_REFLECTION_RDYCPPSCRIPT_H

#include <unordered_map>
#include <utility>
#include <memory>

#include <Dy/Component/Interface/IDyCppScript.h>
#include <Dy/Helper/Pointer.h>

namespace dy::reflect
{

///
/// @brief
/// @return
///
template<typename TType>
std::unique_ptr<IDyCppScript> __Rfc__CreateAsObject() { return std::make_unique<TType>(); }

///
/// @struct __Rfc__ObjectFactory
/// @brief
///
class __Rfc__ObjectFactory
{
public:
  using TValueType  = std::unique_ptr<IDyCppScript>(*)();
  using TMapType    = std::unordered_map<std::string, TValueType>;

  ///
  /// @brief
  /// @param  typeSpecifier
  /// @return
  ///
  static MDY_NODISCARD std::unique_ptr<IDyCppScript> __Rfc__CreateInstance(_MIN_ const std::string& typeSpecifier)
  {
    TMapType::iterator it = __Rfc__GetTypeInitMapReference().find(typeSpecifier);
    if (it == __Rfc__GetTypeInitMapReference().end()) { return nullptr; };

    return it->second();
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
/// @brief Add cpp script type to meta information.
/// @param typeSpecifier Specifier typename.
///
void __Rfc__AddMetaInformation(_MIN_ const std::string& typeSpecifier);

///
/// @struct
/// @brief
///
template <typename TType>
struct __Rfc__DerivedRegister final : public __Rfc__ObjectFactory
{
  __Rfc__DerivedRegister(_MIN_ std::string const& typeSpecifier)
  {
    __Rfc__GetTypeInitMapReference().try_emplace(typeSpecifier, &__Rfc__CreateAsObject<TType>);
    __Rfc__AddMetaInformation(typeSpecifier);
  }
};

} /// ::dy::reflect namespace

namespace dy
{

///
/// @brief  Get cpp script instance using scriptSpecifier.
/// @param  scriptSpecifier Specifier name.
/// @return Script instance.
///
inline MDY_NODISCARD std::unique_ptr<IDyCppScript>
DyRefGetScriptInstance(_MIN_ const std::string& scriptSpecifier)
{
  return reflect::__Rfc__ObjectFactory::__Rfc__CreateInstance(scriptSpecifier);
}

} /// ::dy namespace

#endif /// MDY_CORE_REFLECTION_RDYCPPSCRIPT_H
