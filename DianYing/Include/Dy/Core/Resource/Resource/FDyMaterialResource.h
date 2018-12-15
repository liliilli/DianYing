#ifndef GUARD_DY_CORE_RESOURCE_RESOURCE_FDYMATERIALRESOURCE_H
#define GUARD_DY_CORE_RESOURCE_RESOURCE_FDYMATERIALRESOURCE_H
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
  FDyMaterialResource(_MIN_ const FDyMaterialInformation& informatio);
  ~FDyMaterialResource();

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
    return this->mPtrShaderResource;
  }

  /// @brief Return binded texture resource pointers list.
  MDY_NODISCARD const std::vector<const FDyTextureResource*>& GetBindedTextureResourcePtrList() const noexcept
  {
    return this->mPtrTextureRescList;
  }

private:
  std::string          mSpecifierName    = MDY_INITIALIZE_EMPTYSTR;
  EDyMaterialBlendMode mBlendMode        = EDyMaterialBlendMode::Opaque;

  using TPtrTextureInfoList = std::vector<const FDyTextureResource*>;

  TPtrTextureInfoList      mPtrTextureRescList  = {};
  const FDyShaderResource* mPtrShaderResource   = MDY_INITIALIZE_NULL;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_RESOURCE_FDYMATERIALRESOURCE_H