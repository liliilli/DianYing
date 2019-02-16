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

#include <Dy/Core/Resource/Internal/MaterialType.h>
#include <Dy/Core/Resource/Type/TDyInformationBinder.h>

//!
//! Forward declaration
//!

namespace dy
{
class FDyShaderInformation;
class FDyTextureInformation;
struct PDyMaterialInstanceMetaInfo;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

class FDyMaterialInformation final
{
public:
  using TPtrTextureInfoList = std::vector<std::unique_ptr<TDyLInformationBinderTexture>>;

  MDY_NOT_COPYABLE_MOVEABLE_PROPERTIES(FDyMaterialInformation);
  FDyMaterialInformation(_MIN_ const PDyMaterialInstanceMetaInfo& metaInfo);
  ~FDyMaterialInformation() = default;

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

  /// @brief Get const pointer list of texture information (must be valid).
  MDY_NODISCARD const TPtrTextureInfoList& GetPtrTextureInformationList() const noexcept
  {
    return this->mBinderTextureInfoList;
  }

  /// @brief Get const shader information pointer (must be valid)
  MDY_NODISCARD const auto& GetPtrShaderInformation() const noexcept
  {
    return this->mBinderShaderInfo;
  }

private:
  std::string          mSpecifierName    = MDY_INITIALIZE_EMPTYSTR;
  EDyMaterialBlendMode mBlendMode        = EDyMaterialBlendMode::Opaque;

  TDyLInformationBinderShader  mBinderShaderInfo = {};
  std::vector<std::unique_ptr<TDyLInformationBinderTexture>> mBinderTextureInfoList = {};
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_INFORMATION_FDYMATERIALINFORMAITON_H