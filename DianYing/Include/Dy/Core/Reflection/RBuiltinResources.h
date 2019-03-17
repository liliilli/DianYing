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

#include <Dy/Meta/Type/EResourceType.h>
#include <Dy/Builtin/Interface/IResource.h>

namespace dy::reflect
{

class RBuiltinResources
{
public:
  using TReturnType = std::unique_ptr<IResource>;
  using TValueType = std::pair<EResourceType, TReturnType(*)()>;
  using TMapType = std::unordered_map<std::string, TValueType>;

  inline static unsigned count = 0u;
  static void BindBuiltinResourcesToMetaManager();

protected:
  /// @brief Check booting meta information is exist.
  static bool IsBootingMetaInfoScriptExist() noexcept;

  /// @brief Check loading widget resource meta information is exist.
  static bool IsLoadingWidgetMetaInfoExist() noexcept;

  /// @brief Get resource map reference container.
  static TMapType& GetResourceMapReference();

  /// @brief Loading booting resource script meta information
  static TValueType& GetBootResourceMetaInfo();

  /// @brief Get global resource script meta information list.
  static std::vector<TValueType>& GetGlobalResourceMetaInfo();

  /// @brief Loading widget resource meta information
  static TValueType& GetLoadingWidgetResourceMetaInfo();

  template <typename TType>
  static void RegisterScriptResource(const std::string& specifier);
};

template<typename TType>
RBuiltinResources::TReturnType __Rfc__GetInstance() { return std::make_unique<TType>(); }

template <typename TType>
struct RDyBuiltinResourceRegister final : public RBuiltinResources
{
  inline static bool mFlag = false;
  explicit RDyBuiltinResourceRegister(const std::string_view& name);
};

} /// ::dy::reflect namespace

#endif /// GUARD_DY_CORE_REFLECTION_RDYBUILTINRESOURCES_H
#include <Dy/Core/Reflection/Inline/RBuiltinResourcesTmp.inl>