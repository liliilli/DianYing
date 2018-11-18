#ifndef MDY_CORE_REFLECTION_RDYCPPSCRIPT_H
#define MDY_CORE_REFLECTION_RDYCPPSCRIPT_H

#include <unordered_map>
#include <utility>
#include <memory>

#include <Dy/Component/Interface/IDyScriptable.h>
#include <Dy/Component/Abstract/ADyWidgetCppScript.h>
#include <Dy/Component/Abstract/ADyActorCppScript.h>
#include <Dy/Helper/Pointer.h>
#include <Dy/Meta/Information/ScriptMetaInformation.h>

namespace dy::reflect
{

///
/// @struct __Rfc__ScriptFactory
/// @brief  Script reflection factory.
///
class __Rfc__ScriptFactory
{
public:
  using TReturnType = std::unique_ptr<IDyScriptable>;
  using TValueType  = std::pair<EDyScriptMode, TReturnType(*)()>;
  using TMapType    = std::unordered_map<std::string, TValueType>;

  ///
  /// @brief
  /// @param  typeSpecifier
  /// @return
  ///
  static MDY_NODISCARD std::unique_ptr<IDyScriptable> __Rfc__CreateInstance(_MIN_ const std::string& typeSpecifier)
  {
    TMapType::iterator it = __Rfc__GetTypeInitMapReference().find(typeSpecifier);
    if (it == __Rfc__GetTypeInitMapReference().end()) { return nullptr; };

    return it->second.second();
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
/// @brief
/// @return
///
template<typename TType>
__Rfc__ScriptFactory::TReturnType __Rfc__CreateAsObject() { return std::make_unique<TType>(); }

///
/// @brief Add cpp script type to meta information.
/// @param typeSpecifier Specifier typename.
///
void __Rfc__AddMetaInformation(_MIN_ const std::string& typeSpecifier, _MIN_ const EDyScriptMode mode);

///
/// @struct
/// @brief
///
template <typename TType>
struct __Rfc__DerivedRegister final : public __Rfc__ScriptFactory
{
  __Rfc__DerivedRegister(_MIN_ std::string const& typeSpecifier)
  {
    if constexpr (std::is_base_of_v<ADyWidgetCppScript, TType> == true)
    {
      __Rfc__GetTypeInitMapReference().try_emplace(typeSpecifier, EDyScriptMode::Widget, &__Rfc__CreateAsObject<TType>);
      __Rfc__AddMetaInformation(typeSpecifier, EDyScriptMode::Widget);
    }
    else if constexpr (std::is_base_of_v<ADyActorCppScript, TType> == true)
    {
      __Rfc__GetTypeInitMapReference().try_emplace(typeSpecifier, EDyScriptMode::Actor, &__Rfc__CreateAsObject<TType>);
      __Rfc__AddMetaInformation(typeSpecifier, EDyScriptMode::Actor);
    }
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
inline MDY_NODISCARD reflect::__Rfc__ScriptFactory::TReturnType
DyRefGetScriptInstance(_MIN_ const std::string& scriptSpecifier)
{
  return reflect::__Rfc__ScriptFactory::__Rfc__CreateInstance(scriptSpecifier);
}

///
/// @brief  Get cpp script instance using scriptSpecifier.
/// @param  scriptSpecifier Specifier name.
/// @return Script instance.
///
inline MDY_NODISCARD std::unique_ptr<ADyWidgetCppScript>
DyRefGetWidgetScriptInstance(_MIN_ const std::string& scriptSpecifier)
{
  return DyConvertUniquePtrTo<ADyWidgetCppScript>(reflect::__Rfc__ScriptFactory::__Rfc__CreateInstance(scriptSpecifier));
}

///
/// @brief  Get cpp script instance using scriptSpecifier.
/// @param  scriptSpecifier Specifier name.
/// @return Script instance.
///
inline MDY_NODISCARD std::unique_ptr<ADyActorCppScript>
DyRefGetActorScriptInstance(_MIN_ const std::string& scriptSpecifier)
{
  return DyConvertUniquePtrTo<ADyActorCppScript>(reflect::__Rfc__ScriptFactory::__Rfc__CreateInstance(scriptSpecifier));
}


} /// ::dy namespace

#endif /// MDY_CORE_REFLECTION_RDYCPPSCRIPT_H
