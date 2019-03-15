#ifndef GUARD_DY_CORE_RESOURCE_RESOURCE_FDYMATERIALRESOURCE_H
#define GUARD_DY_CORE_RESOURCE_RESOURCE_FDYMATERIALRESOURCE_H
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
#include <Dy/Core/Resource/Type/TResourceBinder.h>
#include <Dy/Core/Resource/Type/TInformationBinder.h>

//!
//! Forward declaration
//!

namespace dy
{
struct PDyMaterialInstanceMetaInfo;
class FDyMaterialInformation;
class FDyTextureResource;
class FDyShaderResource;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

class FDyMaterialResource final
{
public:
  FDyMaterialResource(_MIN_ const FDyMaterialInformation& information);
  FDyMaterialResource(_MIN_ const PDyMaterialInstanceMetaInfo& iInstanceInfo);
  ~FDyMaterialResource() = default;

  /// @brief Get specifier name of material information.
  MDY_NODISCARD const std::string& GetSpecifierName() const noexcept
  {
    return this->mSpecifierName;
  }

  /// @brief Get blend mode of material information.
  MDY_NODISCARD EMaterialBlendMode GetBlendMode() const noexcept
  {
    return this->mBlendMode;
  }

  /// @brief
  MDY_NODISCARD const FDyShaderResource* GetShaderResourcePtr() const noexcept
  {
    return this->mBinderShader.Get();
  }

  /// @brief
  MDY_NODISCARD auto& GetShaderResourceBinder() noexcept { return this->mBinderShader; }

  /// @brief Return binded texture resource pointers list.
  MDY_NODISCARD const auto& GetBindedTextureResourcePtrList() const noexcept
  {
    return this->mBinderTextureList;
  }

  /// @brief Update texture list using shader. \n
  /// When either texture list or material or shader is not exist, just do nothing but return DY_FAILURE.
  EDySuccess TryUpdateTextureList() noexcept;

  /// @brief Detach texture list which is bound to present material shader.
  /// When either texture list or material or shader is not exist, just do nothing but return DY_FAILURE.
  EDySuccess TryDetachTextureListFromShader() noexcept;

private:
  std::string          mSpecifierName    = MDY_INITIALIZE_EMPTYSTR;
  EMaterialBlendMode mBlendMode        = EMaterialBlendMode::Opaque;

  TDyInformationBinderMaterial mBinderMaterial;
  TDyResourceBinderShader      mBinderShader;
  std::vector<std::unique_ptr<TDyResourceBinderTexture>> mBinderTextureList = {};

  bool mIsInstant = false;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_RESOURCE_FDYMATERIALRESOURCE_H