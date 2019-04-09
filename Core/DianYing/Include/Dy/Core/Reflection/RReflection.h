#ifndef GUARD_DY_CORE_REFLECTION_INLINE_RREFLECTION_H
#define GUARD_DY_CORE_REFLECTION_INLINE_RREFLECTION_H
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

#include <unordered_map>
#include <Dy/Core/Reflection/Internal/EReflectionType.h>
#include <Dy/Helper/System/Assertion.h>

namespace dy::reflect
{
  
/// @struct DType
/// @brief Runtime type information structure.
struct DType final
{
  /// @struct DMember
  /// @brief Runtime type member varaible information structrue.
  struct DMember final
  {
    EReflectScopeType mType = EReflectScopeType::__Error;
    size_t mOffset = 0;
  };
 
  /// @brief The size of type.
  size_t mTypeSize = 0;
  /// @brief Alias of given type.
  std::string mAlias = "";
  /// @brief Bound member variable information map.
  std::unordered_map<std::string, DMember> mMemberVariables;
  /// @brief Alias container.
  std::unordered_map<std::string, std::string> mAliasContainer;
  
  /// @brief
  EReflectScopeType GetTypeOf(const std::string& iVariable) const;

  /// @brief
  size_t GetOffsetOf(const std::string& iVariable) const;
  
  /// @brief
  template <typename TType>
  size_t GetAddressOf(const TType& iInstance, const std::string& iVariable) const; 
  
  /// @brief
  template <typename TValueType, typename TType>
  TValueType& GetValueOf(TType& iInstance, const std::string& iVariable) const;
  
  /// @brief
  template <typename TValueType, typename TType>
  const TValueType& GetValueOf(const TType& iInstance, const std::string& iVariable) const;

  /// @brief
  bool IsAliasExist(const std::string& iAlias) const;

  /// @brief
  const std::string& GetVarNameOf(const std::string& iAlias) const;
};

template <typename TType> struct RRegisterType;

/// @struct RUniformReflection
/// @brief Reflection container for universe & uniform POD-like structure.
struct RUniformReflection
{
protected:
  using TAliasMap = std::unordered_map<std::string, std::string>;
  using TTypeMap  = std::unordered_map<std::string, DType>; 

  static TAliasMap& GetAliasContainer() noexcept
  {
    static TAliasMap map;
    return map;
  }

  static TTypeMap& GetTypeContainer() noexcept
  {
    static TTypeMap map;
    return map;
  }

public:
  static size_t GetSizeOf(const std::string& iType)
  {
    return GetTypeContainer().at(iType).mTypeSize; 
  }

  static const DType& GetData(const std::string& iType)
  {
    return GetTypeContainer().at(iType);
  }

  static void RegisterSubName(const std::string& iSubName, const std::string& iTypeName)
  {
    GetAliasContainer()[iSubName] = iTypeName;
  }

  static const std::string& GetTypeNameOf(const std::string& iAlias)
  {
    return GetAliasContainer()[iAlias];
  }

  static const std::string& GetFirstAliasOf(const std::string& iTypeName)
  {
    const auto& container = GetAliasContainer();
    for (const auto& [alias, typeName] : container)
    {
      if (typeName == iTypeName) { return alias; }
    }

    MDY_UNEXPECTED_BRANCH();
    throw 0; // This is intentional.
  }

  static bool IsSubNameExist(const std::string& iSubName)
  {
    return GetAliasContainer().find(iSubName) != GetAliasContainer().end();
  }

  static const DType& GetDataOfSubName(const std::string& iSubName)
  {
    return GetTypeContainer().at(GetAliasContainer().at(iSubName));
  }
  
  template <typename TType>
  static RRegisterType<TType> Register(const std::string& iTypeName);
};

template <typename TType>
struct RRegisterType final : public RUniformReflection
{
  inline static std::string sTypeName;
  using Type = TType;

  RRegisterType(const std::string& iTypeName, const size_t iStructSize);
  ~RRegisterType() = default;
  
  template <typename TVariableType>
  RRegisterType& RegisterVariable(const std::string& iVariableName, size_t iOffset);

  RRegisterType& RegsiterTypeSubName(const std::string& iSubName);

  RRegisterType& RegisterVariableSubname(const std::string& iSubName, const std::string& iVarName);
};


template <typename TType>
inline RRegisterType<TType> RUniformReflection::Register(const std::string& iTypeName)
{
  return RRegisterType<TType>{iTypeName, sizeof(TType)};
}

template <typename TType>
template <typename TVariableType>
inline RRegisterType<TType>& RRegisterType<TType>::RegisterVariable(const std::string& iVariableName, size_t iOffset)
{
  static constexpr auto type = ToVariableType<TVariableType>::Value;
  GetTypeContainer()[sTypeName].mMemberVariables[iVariableName] = DType::DMember{type, iOffset};
  
  return *this;
}

template <typename TType>
RRegisterType<TType>& RRegisterType<TType>::RegsiterTypeSubName(const std::string& iSubName)
{
  GetTypeContainer()[sTypeName].mAlias = iSubName;
  RegisterSubName(iSubName, sTypeName);
  return *this;
}

template <typename TType>
RRegisterType<TType>& RRegisterType<TType>::RegisterVariableSubname(
  const std::string& iSubName, const std::string& iVarName)
{
  auto& data = GetTypeContainer()[sTypeName];
  data.mAliasContainer[iSubName] = iVarName;
  return *this;
}

#define MDy_ReflectionStartName MDY_MAKENAME(__, ReflectionStart)
struct MDy_ReflectionStartName final
{ 
  template<typename TType> 
  MDy_ReflectionStartName(RRegisterType<TType>) {} 
};

} /// ::dy::reflect namespace

/// @define MDyReflectionStart
/// @brief 
#define MDyReflectionStart \
static ::dy::reflect::MDy_ReflectionStartName MDY_MAKENAME(_____, __LINE__)

/// @define REGISTER_TYPE
/// @brief 
#define REGISTER_TYPE(__Type__, __Alias__) \
  ::dy::reflect::RUniformReflection::Register<__Type__>(#__Type__).RegsiterTypeSubName(#__Alias__)

/// @define REGISTER_VARAIBLE
/// @brief
#define REGISTER_VARIABLE(__Type__, __Variable__, __Alias__) \
  RegisterVariable<decltype(__Type__::__Variable__)>(#__Variable__, offsetof(__Type__, __Variable__)) \
  .RegisterVariableSubname(#__Alias__, #__Variable__)

/// @define REGISTER_UNIFORM_STRUCT
/// @brief
#define REGISTER_UNIFORM_STRUCT(__Type__) \
  static constexpr const char* __sTypeName = #__Type__;

/* Example for registering type information.
struct DTest
{
  std::string a = "Hello";
  float c = 3.14159f;
  int   d = 169;
  std::string b = "World";
};

MDyReflectionStart
{
  REGISTER_TYPE(DTest, dtest)
    .REGISTER_VARIABLE(DTest, a, alias_a)
    .REGISTER_VARIABLE(DTest, b, alias_b)
    .REGISTER_VARIABLE(DTest, c, alias_c)
    .REGISTER_VARIABLE(DTest, d, alias_d)
};
 */

/* Example of access to run-time information.
int main()
{
  DTest test;

  const auto& data = RUniformReflection::GetData("DTest");
  std::cout << data.GetValueOf<float>(test, "c") << std::endl;
  std::cout << data.GetValueOf<std::string>(test, data.GetVarNameOf("alias_a")) << std::endl;
  std::cout << data.GetValueOf<std::string>(test, "b") << std::endl;
  std::cout << data.GetValueOf<int>(test, "d") << std::endl;
  std::cout << data.GetValueOf<std::complex<float>>(test, "mComplex") << std::endl;
}
 */

#endif /// GUARD_DY_CORE_REFLECTION_INLINE_RREFLECTION_H
#include <Dy/Core/Reflection/Inline/RReflectionTmp.inl>
