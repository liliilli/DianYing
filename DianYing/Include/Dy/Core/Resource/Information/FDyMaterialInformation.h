#ifndef GUARD_DY_CORE_RESOURCE_INFORMATION_FDYMATERIALINFORMAITON_H
#define GUARD_DY_CORE_RESOURCE_INFORMATION_FDYMATERIALINFORMAITON_H
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
  using TPtrTextureInfoList = std::vector<NotNull<const FDyTextureInformation*>>;

  MDY_NOT_COPYABLE_MOVEABLE_PROPERTIES(FDyMaterialInformation);
  FDyMaterialInformation(_MIN_ const PDyMaterialInstanceMetaInfo& metaInfo);
  ~FDyMaterialInformation();

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
    return this->mPtrTextureInfoList;
  }

  /// @brief Get const shader information pointer (must be valid)
  MDY_NODISCARD NotNull<const FDyShaderInformation*> GetPtrShaderInformation() const noexcept
  {
    return DyMakeNotNull(this->mPtrShaderInfo);
  }

private:
  std::string          mSpecifierName    = MDY_INITIALIZE_EMPTYSTR;
  EDyMaterialBlendMode mBlendMode        = EDyMaterialBlendMode::Opaque;

  TPtrTextureInfoList         mPtrTextureInfoList = {};
  const FDyShaderInformation* mPtrShaderInfo      = MDY_INITIALIZE_NULL;

  TDyLInformatinBinderShader  mBinderShaderInfo = {};
  std::vector<std::unique_ptr<TDyIInformationBinderTexture>> mBinderTextureInfoList = {};
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_INFORMATION_FDYMATERIALINFORMAITON_H