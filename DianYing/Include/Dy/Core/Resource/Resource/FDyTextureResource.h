#ifndef GUARD_DY_CORE_RESOURCE_RESOURCE_FDYTEXTURERESOURCE_H
#define GUARD_DY_CORE_RESOURCE_RESOURCE_FDYTEXTURERESOURCE_H
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

#include <Dy/Helper/Type/DVector2.h>
#include <Dy/Core/Resource/Internal/ETextureEnums.h>
#include <Dy/Core/Resource/Type/TInformationBinder.h>

//!
//! Forward declaration
//!

namespace dy
{
class FDyTextureInformation;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

///
/// @class FDyTextureResource
/// @brief Texture resource class that serve heap instance.
///
class FDyTextureResource 
{
public:
  FDyTextureResource(_MIN_ const FDyTextureInformation& information);
  virtual ~FDyTextureResource() = 0;

  /// @brief Get specifier name of valid shader resource.
  MDY_NODISCARD const std::string& GetSpecifierName() const noexcept;

  /// @brief Get binded texture resource id.
  MDY_NODISCARD TU32 GetTextureId() const noexcept;

  /// @brief Get texture type value.
  MDY_NODISCARD ETextureStyleType GetTextureType() const noexcept;

protected:
  std::string         mTextureName = MDY_INITIALIZE_EMPTYSTR;
  ETextureStyleType mTextureType = ETextureStyleType::NoneError;

  TU32 mTextureResourceId = MDY_INITIALIZE_DEFUINT;
  TDyInformationBinderTexture mInformationBinder;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_RESOURCE_FDYTEXTURERESOURCE_H