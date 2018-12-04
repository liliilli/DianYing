#ifndef GUARD_DY_SHADER_INFORMATION_H
#define GUARD_DY_SHADER_INFORMATION_H
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

#include <Dy/Core/Resource/Internal/ShaderType.h>
#include <Dy/Helper/Pointer.h>

//!
//! Forward declaration
//!

namespace dy
{
struct PDyGLShaderInstanceMetaInfo;
class CDyShaderResource_Deprecated;
}

//!
//! Implementation
//!

namespace dy
{

///
/// @class DDyShaderInformation_Deprecated
/// @brief Shader information class that not serve heap instance
/// but information to create heap instance.
///
class DDyShaderInformation_Deprecated final
{
public:
  MDY_NOT_COPYABLE_MOVEABLE_PROPERTIES(DDyShaderInformation_Deprecated);
  DDyShaderInformation_Deprecated(const PDyShaderConstructionDescriptor& shaderConstructionDescriptor);
  DDyShaderInformation_Deprecated(const PDyGLShaderInstanceMetaInfo& shaderMetaInfo);
  ~DDyShaderInformation_Deprecated();

  ///
  /// @brief return immutable descriptor information reference.
  ///
  [[nodiscard]]
  FORCEINLINE const PDyShaderConstructionDescriptor& GetInformation() const noexcept
  {
    return this->mShaderInformation;
  }

  ///
  /// @brief Check if object is being binded to CDyTextureResource_Deprecated instance.
  ///
  [[nodiscard]]
  FORCEINLINE bool IsBeingBindedToResource() const noexcept
  {
    return this->__mLinkedShaderResourcePtr != nullptr;
  }

private:
  PDyShaderConstructionDescriptor mShaderInformation;

  //!
  //! Resource pointers binding
  //!

  void __pfSetShaderResourceLink(NotNull<CDyShaderResource_Deprecated*> ptr) const noexcept;
  FORCEINLINE void __pfResetShaderResourceLink() const noexcept
  {
    this->__mLinkedShaderResourcePtr = nullptr;
  }

  MDY_TRANSIENT CDyShaderResource_Deprecated* __mLinkedShaderResourcePtr = nullptr;

  friend class CDyShaderResource_Deprecated;
  friend class MDyIOResource;
};

} /// ::dy namespace

#endif /// GUARD_DY_SHADER_INFORMATION_H