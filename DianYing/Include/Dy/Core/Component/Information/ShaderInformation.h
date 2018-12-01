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

#include <Dy/Core/Component/Internal/ShaderType.h>
#include <Dy/Helper/Pointer.h>

//!
//! Forward declaration
//!

namespace dy
{
struct PDyGLShaderInstanceMetaInfo;
class CDyShaderResource;
}

//!
//! Implementation
//!

namespace dy
{

///
/// @class DDyShaderInformation
/// @brief Shader information class that not serve heap instance
/// but information to create heap instance.
///
class DDyShaderInformation final
{
public:
  MDY_NOT_COPYABLE_MOVEABLE_PROPERTIES(DDyShaderInformation);
  DDyShaderInformation(const PDyShaderConstructionDescriptor& shaderConstructionDescriptor);
  DDyShaderInformation(const PDyGLShaderInstanceMetaInfo& shaderMetaInfo);
  ~DDyShaderInformation();

  ///
  /// @brief return immutable descriptor information reference.
  ///
  [[nodiscard]]
  FORCEINLINE const PDyShaderConstructionDescriptor& GetInformation() const noexcept
  {
    return this->mShaderInformation;
  }

  ///
  /// @brief Check if object is being binded to CDyTextureResource instance.
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

  void __pfSetShaderResourceLink(NotNull<CDyShaderResource*> ptr) const noexcept;
  FORCEINLINE void __pfResetShaderResourceLink() const noexcept
  {
    this->__mLinkedShaderResourcePtr = nullptr;
  }

  MDY_TRANSIENT CDyShaderResource* __mLinkedShaderResourcePtr = nullptr;

  friend class CDyShaderResource;
  friend class MDyIOResource;
};

} /// ::dy namespace

#endif /// GUARD_DY_SHADER_INFORMATION_H