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

#include <nlohmann/json_fwd.hpp>
#include <Dy/Helper/Type/VectorInt2.h>
#include <Dy/Meta/Information/CommonResourceMetaInfo.h>
#include <Dy/Element/Interface/IDyToString.h>
#include <Dy/Core/Resource/Internal/MaterialType.h>
#include <Dy/Core/Resource/Internal/TextureEnums.h>

namespace dy
{

struct DDyMaterialTextureItem final
{
  std::string       mTextureSpecifier = "";
  EDyTextureMapType mTextureMapType   = EDyTextureMapType::Unknown;
};

/// @brief Serialization function.
void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const DDyMaterialTextureItem& p);
/// @brief Deserialization function.
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ DDyMaterialTextureItem& p);

///
/// @struct PDyMaterialInstanceMetaInfo
/// @brief Descriptor instance which saves information to create material information.
///
struct PDyMaterialInstanceMetaInfo final : public PDyCommonResourceMetaInfo, public IDyToString
{
  using TTextureList = std::array<DDyMaterialTextureItem, 16>;

  std::string                 mSpecifierName    = MDY_INITIALIZE_EMPTYSTR;
  std::string                 mShaderSpecifier  = MDY_INITIALIZE_EMPTYSTR;
  EDyMaterialBlendMode        mBlendMode        = EDyMaterialBlendMode::Opaque;
  TTextureList                mTextureNames{};

  ///
  /// @brief Return information string.
  ///
  /// PDyMaterialInstanceMetaInfo
  /// Material Name : ""
  /// Shader Name : ""
  /// Texture Name (0) : ""
  /// ...
  /// Texture Name (N) : ""
  /// Blend Mode : ""
  /// Is shader lazy initialized : False / True
  ///
  MDY_NODISCARD std::string ToString() override final;
};

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyMaterialInstanceMetaInfo& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyMaterialInstanceMetaInfo& p);

} /// ::dy namespace

#endif /// GUARD_DY_META_INFORMATION_METAINFOMATERIAL_H