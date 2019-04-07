#ifndef GUARD_DY_META_INFORMATION_METAINFOMATERIAL_H
#define GUARD_DY_META_INFORMATION_METAINFOMATERIAL_H
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
#include <memory>
#include <nlohmann/json_fwd.hpp>

#include <Dy/Core/Resource/Internal/EMaterialBlendMode.h>
#include <Dy/Core/Resource/Internal/ETextureEnums.h>
#include <Dy/Core/Resource/Internal/Uniform/IUniformValueBase.h>
#include <Dy/Element/Interface/IToString.h>
#include <Dy/Helper/Type/DVector2.h>
#include <Dy/Meta/Information/CommonResourceMetaInfo.h>
#include <Dy/Core/Resource/Type/Shader/XTemplateUniformType.h>

namespace dy
{

struct DDyMaterialTextureItem final
{
  std::string       mTextureSpecifier = "";
  ETextureMapType mTextureMapType   = ETextureMapType::Unknown;
};

/// @brief Serialization function.
void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const DDyMaterialTextureItem& p);
/// @brief Deserialization function.
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ DDyMaterialTextureItem& p);

/// @struct PDyMaterialInstanceMetaInfo
/// @brief Descriptor instance which saves information to create material information.
struct PDyMaterialInstanceMetaInfo final : public PDyCommonResourceMetaInfo, public IToString
{
  using TTextureList = std::array<DDyMaterialTextureItem, 16>;
  using TUniformList = std::unordered_map<std::string, std::shared_ptr<IUniformValueBase>>;

  std::string          mSpecifierName;
  std::string          mShaderSpecifier;
  EMaterialBlendMode mBlendMode = EMaterialBlendMode::Opaque;
  TTextureList         mTextureNames;
  /// @brief Uniform values but not specifies id but initial value.
  TUniformList         mUniformValues;       

  ///
  /// @brief Return information string.
  ///
  /// PDyMaterialInstanceMetaInfo
  /// Material Name : ""
  /// Shader Name : ""
  /// Blend Mode : ""
  /// Uniform Values(0) : "", Type : "", Value : ""
  /// ...
  /// Uniform Values(N) : "", Type : "", Value : ""
  /// Texture Name (0) : ""
  /// ...
  /// Texture Name (N) : ""
  ///
  MDY_NODISCARD std::string ToString() override final;

  template <EUniformVariableType TType>
  static void InsertValue(
    PDyMaterialInstanceMetaInfo& ioMaterialInfo, 
    const std::string& iSpecifier, 
    const typename MDY_PRIVATE(UniformBinder)<TType>::ValueType& iValue);
};

void to_json(nlohmann::json& oJson, const PDyMaterialInstanceMetaInfo& iMaterial);
void from_json(const nlohmann::json& iJson, PDyMaterialInstanceMetaInfo& iMaterial);

} /// ::dy namespace

#endif /// GUARD_DY_META_INFORMATION_METAINFOMATERIAL_H
#include <Dy/Meta/Information/Inline/DMetaMetarialInstanceTmp.inl>