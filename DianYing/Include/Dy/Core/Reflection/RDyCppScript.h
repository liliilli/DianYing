#ifndef MDY_CORE_REFLECTION_RDYCPPSCRIPT_H
#define MDY_CORE_REFLECTION_RDYCPPSCRIPT_H

#include <unordered_map>
#include <utility>
#include <memory>

namespace dy::reflect
{

struct Object {};

///
/// @brief
/// @return
///
template<typename TType>
std::unique_ptr<Object> __Rfc__CreateAsObject() { return std::make_unique<TType>(); }

///
/// @struct __Rfc__ObjectFactory
/// @brief
///
class __Rfc__ObjectFactory
{
public:
  using TValueType  = std::unique_ptr<Object>(*)();
  using TMapType    = std::unordered_map<std::string, TValueType>;

  ///
  /// @brief
  /// @param  typeSpecifier
  /// @return
  ///
  static MDY_NODISCARD std::unique_ptr<Object> __Rfc__CreateInstance(_MIN_ const std::string& typeSpecifier)
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
/// @struct
/// @brief
///
template <typename TType>
struct __Rfc__DerivedRegister final : public __Rfc__ObjectFactory
{
  __Rfc__DerivedRegister(_MIN_ std::string const& typeSpecifier)
  {
    __Rfc__GetTypeInitMapReference().try_emplace(typeSpecifier, &__Rfc__CreateAsObject<TType>);
  }
};

} /// ::dy::reflect namespace

class Test_DerivedB : public dy::reflect::Object
{
  MDY_REFLECT_ENABLE(Test_DerivedB);
public:
  int operator()() { return 65536; }
};

#ifdef false
int main()
{
  auto instance = MDY_REFLECT_GETINSTANCE_AS(DerivedB);

  const auto i = (*instance)();
  std::cout << i << std::endl;
  return i;
}
#endif

#endif /// MDY_CORE_REFLECTION_RDYCPPSCRIPT_H
