#ifndef GUARD_DY_CORE_RESOURCE_RESOURCE_FDYTEXTURERESOURCE_H
#define GUARD_DY_CORE_RESOURCE_RESOURCE_FDYTEXTURERESOURCE_H
#include "Dy/Helper/Type/VectorInt2.h"
#include "Dy/Core/Resource/Internal/TextureEnums.h"

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
class FDyTextureResource final
{
public:
  FDyTextureResource(_MIN_ const FDyTextureInformation& information);
  ~FDyTextureResource();

  /// @brief Get specifier name of valid shader resource.
  MDY_NODISCARD const std::string& GetSpecifierName() const noexcept
  {
    return this->mTextureName;
  }

  /// @brief Get binded texture resource id.
  MDY_NODISCARD FORCEINLINE TU32 GetTextureId() const noexcept
  {
    return this->mTextureResourceId;
  };

  /// @brief Get texture type value.
  MDY_NODISCARD FORCEINLINE EDyTextureStyleType GetTextureType() const noexcept
  {
    return this->mTextureType;
  }

private:
  std::string   mTextureName        = MDY_INITIALIZE_EMPTYSTR;
  DDyVectorInt2 mTextureSize        = {};
  EDyTextureStyleType mTextureType  = EDyTextureStyleType::NoneError;

  TU32          mTextureResourceId  = MDY_INITIALIZE_DEFUINT;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_RESOURCE_FDYTEXTURERESOURCE_H