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

#include <Dy/Core/Resource/Internal/MaterialType.h>
#include <Dy/Core/Resource/Type/TDyResourceBinder.h>
#include <Dy/Core/Resource/Type/TDyInformationBinder.h>

//!
//! Forward declaration
//!

namespace dy
{
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
  ~FDyMaterialResource() = default;

  /// @brief Get specifier name of material information.
  MDY_NODISCARD const std::string& GetSpecifierName() const noexcept
  {
    return this->mSpecifierName;
  }

  /// @brief Get blend mode of material information.
  MDY_NODISCARD EDyMaterialBlendMode GetBlendMode() const noexcept
  {
    return this->mBlendMode;
  }

  /// @brief
  MDY_NODISCARD const FDyShaderResource* GetShaderResourcePtr() const noexcept
  {
    return this->mBinderShader.Get();
  }

  /// @brief
  MDY_NODISCARD auto& GetShaderResourceBinder() noexcept { return this->mBinderShader;
  }


  /// @brief Return binded texture resource pointers list.
  MDY_NODISCARD const auto& GetBindedTextureResourcePtrList() const noexcept
  {
    return this->mBinderTextureList;
  }

private:
  std::string          mSpecifierName    = MDY_INITIALIZE_EMPTYSTR;
  EDyMaterialBlendMode mBlendMode        = EDyMaterialBlendMode::Opaque;

  TDyIInformationBinderMaterial mBinderMaterial;
  TDyIResourceBinderShader      mBinderShader;
  std::vector<std::unique_ptr<TDyIResourceBinderTexture>> mBinderTextureList = {};
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_RESOURCE_FDYMATERIALRESOURCE_H