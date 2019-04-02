#ifndef GUARD_DY_CORE_RESOURCE_INFORMATION_FDYMATERIALINFORMAITON_H
#define GUARD_DY_CORE_RESOURCE_INFORMATION_FDYMATERIALINFORMAITON_H
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

#include <Dy/Core/Resource/Internal/EMaterialBlendMode.h>
#include <Dy/Core/Resource/Type/TInformationBinder.h>

//!
//! Forward declaration
//!

namespace dy
{
class FInformationShader;
class FInformationTexture;
struct PDyMaterialInstanceMetaInfo;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

class FInformationMaterial final
{
public:
  using TPtrTextureInfoList = std::vector<std::unique_ptr<TDyInformationBinderTexture>>;

  MDY_NOT_COPYABLE_MOVEABLE_PROPERTIES(FInformationMaterial);
  FInformationMaterial(const PDyMaterialInstanceMetaInfo& metaInfo, bool iIsInstanced);
  ~FInformationMaterial() = default;

  /// @brief Get specifier name of material information.
  MDY_NODISCARD const std::string& GetSpecifierName() const noexcept;

  /// @brief Get blend mode of material information.
  MDY_NODISCARD EMaterialBlendMode GetBlendMode() const noexcept;

  /// @brief Get const pointer list of texture information (must be valid).
  MDY_NODISCARD const TPtrTextureInfoList& GetPtrTextureInformationList() const noexcept;

  /// @brief Get const shader information pointer (must be valid)
  MDY_NODISCARD const TDyInformationBinderShader& GetPtrShaderInformation() const noexcept;

  /// @brief Check this material supports instancing.
  MDY_NODISCARD bool IsInstanced() const noexcept;

private:
  std::string        mSpecifierName;
  EMaterialBlendMode mBlendMode = EMaterialBlendMode::Opaque;

  TDyInformationBinderShader mBinderShaderInfo = {};
  std::vector<std::unique_ptr<TDyInformationBinderTexture>> mBinderTextureInfoList = {};

  /// @brief Check this material instance is instancing material.
  bool mIsInstanced = false;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_INFORMATION_FDYMATERIALINFORMAITON_H