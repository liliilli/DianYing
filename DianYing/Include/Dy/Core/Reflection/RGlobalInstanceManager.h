#ifndef GUARD_DY_CORE_REFLECTION_RGlobalInstanceManager_H
#define GUARD_DY_CORE_REFLECTION_RGlobalInstanceManager_H
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
#include <Dy/Builtin/Abstract/ADyGlobalInstance.h>
#include <Dy/Helper/System/HashCompileCrc32.h>
#include <Dy/Helper/System/TypeTraits.h>
#include <Dy/Helper/System/Macroes.h>

namespace dy
{

template <hash::THashVal32 THashVal>
class TDyGlobalInstance;

}

namespace dy::reflect
{

/// @class RGlobalInstanceManager
/// @brief Global instance manager.
class RGlobalInstanceManager
{
public:
  using TMapType = std::unordered_map<hash::THashVal32, std::unique_ptr<ADyGlobalInstance>>;

  /// @brief Check global instance is exist.
  [[nodiscard]] static bool IsGlobalInstanceExist(const char* iSpecifier);
  /// @brief Check global instance is exist.
  [[nodiscard]] static bool IsGlobalInstanceExist(const std::string& iSpecifier);

  /// @brief Get pointer of global instance with specifier name. This function is general.
  [[nodiscard]] ADyGlobalInstance* GetGlobalInstance(const char* iSpecifier) noexcept;
  /// @brief Get pointer of global instance with specifier name. This function is general.
  [[nodiscard]] ADyGlobalInstance* GetGlobalInstance(const std::string& iSpecifier) noexcept;

protected:
  /// @brief Get map.
  [[nodiscard]] static TMapType& GetInstanceMap() { static TMapType typeMap; return typeMap; }

public:
  /// @brief Get templated pointer of global instance with specifier name.
  template <hash::THashVal32 THashVal>
  [[nodiscard]] static typename TDyGlobalInstance<THashVal>::Type* GetGlobalInstance() noexcept
  {
    using TConversionType = typename TDyGlobalInstance<THashVal>::Type;

    auto& map = RGlobalInstanceManager::GetInstanceMap();
    const auto it = map.find(THashVal);
    if (it == map.end()) { return nullptr; } else { return static_cast<TConversionType*>(it->second.get()); }
  }
};

/// @struct RGlobalInstanceRegistration
/// @brief Global intance registration type.
template <typename TType>
struct RGlobalInstanceRegistration final : public RGlobalInstanceManager
{
  inline static bool mFlag = false;
  RGlobalInstanceRegistration(hash::THashVal32 iVal)
  {
    if (mFlag == true) { return; }

    static_assert(IsInheritancedFrom<TType, ADyGlobalInstance> == true, "TType must be IResource derived type.");
    GetInstanceMap().try_emplace(iVal, std::make_unique<TType>());

    mFlag = true;
  }
};

/// @macro MDY_GET_GLOBALINSTANCE
/// @brief Get pointer of global instance, __MAStringLiteral__ global instance type.
/// This macro is null-safe and compile time checking when __MAStringLiteral__ can be processed in compile time.
#define MDY_GET_GLOBALINSTANCE(__MAStringLiteral__) \
  ::dy::reflect::RGlobalInstanceManager::GetGlobalInstance<::dy::hash::DyToCrc32Hash(MDY_TO_STRING(__MAStringLiteral__))>()

} /// ::dy::reflect namespace

#endif /// GUARD_DY_CORE_REFLECTION_RGlobalInstanceManager_H