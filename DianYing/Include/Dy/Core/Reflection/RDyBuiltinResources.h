#ifndef GUARD_DY_CORE_REFLECTION_RDYBUILTINRESOURCES_H
#define GUARD_DY_CORE_REFLECTION_RDYBUILTINRESOURCES_H
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

#include <memory>
#include <any>
#include <unordered_map>

#include <Dy/Helper/System/TypeTraits.h>
#include <Dy/Meta/Type/EDyResourceType.h>
#include <Dy/Builtin/Interface/IDyResource.h>
#include <Dy/Builtin/Abstract/ADyLoadingBootResource.h>
#include <Dy/Builtin/Abstract/ADyLoadingGlobalResource.h>
#include <Dy/Builtin/Abstract/ADyLoadingWidgetMetaResource.h>

namespace dy::reflect
{

class RDyBuiltinResource
{
public:
  using TReturnType = std::unique_ptr<IDyResource>;
  using TValueType = std::pair<EDyResourceType, TReturnType(*)()>;
  using TMapType = std::unordered_map<std::string, TValueType>;

  inline static unsigned count = 0u;
  static void BindBuiltinResourcesToMetaManager();

protected:
  /// @brief Check booting meta information is exist.
  static bool IsBootingMetaInfoScriptExist() noexcept { return MDY_CHECK_ISNOTNULL(GetBootResourceMetaInfo().second); }

  /// @brief Check loading widget resource meta information is exist.
  static bool IsLoadingWidgetMetaInfoExist() noexcept { return MDY_CHECK_ISNOTNULL(GetLoadingWidgetResourceMetaInfo().second); }

  static TMapType& GetResourceMapReference()
  {
    static TMapType typeMap;
    return typeMap;
  }

  /// @brief Loading booting resource script meta information
  static TValueType& GetBootResourceMetaInfo()
  {
    static TValueType mLoadingBootingMetaInfoCustomized = {};
    return mLoadingBootingMetaInfoCustomized;
  }

  /// @brief Get global resource script meta information list.
  static std::vector<TValueType>& GetGlobalResourceMetaInfo()
  {
    static std::vector<TValueType> mLoadingGlobalMetaInfoCustomizedList = {};
    return mLoadingGlobalMetaInfoCustomizedList;
  }

  /// @brief Loading widget resource meta information
  static TValueType& GetLoadingWidgetResourceMetaInfo()
  {
    static TValueType mLoadingWidgetResourceMetaInfo = {};
    return mLoadingWidgetResourceMetaInfo;
  }

  template <typename TType>
  static void RegisterScriptResource(_MIN_ const std::string& specifier);
};

template<typename TType>
RDyBuiltinResource::TReturnType __Rfc__GetInstance() { return std::make_unique<TType>(); }

template <typename TType>
struct RDyBuiltinResourceRegister final : public RDyBuiltinResource
{
  inline static bool mFlag = false;
  RDyBuiltinResourceRegister(const std::string_view& name)
  {
    if (mFlag == true) { return; }

    static_assert(IsInheritancedFrom<TType, IDyResource> == true, "TType must be IResource derived type.");
    std::string specifier = name.data(); specifier += std::to_string(count);

    if constexpr (TType::value == EDyResourceType::Script) 
    { this->template RegisterScriptResource<TType>(specifier); }
    else if constexpr (IsInheritancedFrom<TType, ADyLoadingWidgetMetaResource> == true)
    { GetLoadingWidgetResourceMetaInfo() = std::make_pair(TType::value, &__Rfc__GetInstance<TType>); }
    else 
    { GetResourceMapReference().try_emplace(specifier, TType::value, &__Rfc__GetInstance<TType>); }

    count += 1;
    mFlag = true;
  }
};

template <typename TType>
void RDyBuiltinResource::RegisterScriptResource(_MIN_ const std::string& specifier)
{
  if constexpr (IsBaseClassOf<TType, ADyLoadingBootResource> == true)
  { // Bind boot resource script to container.
    GetBootResourceMetaInfo() = std::make_pair(TType::value, &__Rfc__GetInstance<TType>);
  }
  else if constexpr (IsBaseClassOf<TType, ADyLoadingGlobalResource> == true)
  { // Add global loading resource script to container.
    GetGlobalResourceMetaInfo().emplace_back(TType::value, &__Rfc__GetInstance<TType>);
  }
  else
  { // Bind general (widget, actor) script to container.
    GetResourceMapReference().try_emplace(specifier, TType::value, &__Rfc__GetInstance<TType::__ConstructionHelper>);
  }
}

} /// ::dy::reflect namespace

#endif /// GUARD_DY_CORE_REFLECTION_RDYBUILTINRESOURCES_H