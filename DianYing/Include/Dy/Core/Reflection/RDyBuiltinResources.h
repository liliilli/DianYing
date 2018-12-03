#ifndef GUARD_DY_CORE_REFLECTION_RDYBUILTINRESOURCES_H
#define GUARD_DY_CORE_REFLECTION_RDYBUILTINRESOURCES_H
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

#include <memory>
#include <any>
#include <unordered_map>

#include <Dy/Helper/System/TypeTraits.h>
#include <Dy/Builtin/Interface/IDyResource.h>

namespace dy
{

enum class EDyResourceType
{
  Script,
  Model,
  GLShader,
  Texture,
  Material,
  WidgetMeta,
  NoneError
};

}

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
  static TMapType& GetResourceMapReference()
  {
    static TMapType typeMap;
    return typeMap;
  }

  static std::unique_ptr<IDyResource> CreateInstance(const std::string& typeSpecifier)
  {
    auto it = GetResourceMapReference().find(typeSpecifier);
    if (it == GetResourceMapReference().end()) { return nullptr; };

    return it->second.second();
  }
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
    {
      GetResourceMapReference().try_emplace(specifier, TType::value, &__Rfc__GetInstance<TType::__ConstructionHelper>);
    }
    else
    {
      GetResourceMapReference().try_emplace(specifier, TType::value, &__Rfc__GetInstance<TType>);
    }

    count += 1;
    mFlag = true;
  }
};

} /// ::dy::reflect namespace

#endif /// GUARD_DY_CORE_REFLECTION_RDYBUILTINRESOURCES_H