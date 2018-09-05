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

//!
//! Forward declaration
//!

namespace dy
{
class CDyShaderResource;
}

//!
//! Implementation
//!

namespace dy
{

///
/// @class CDyShaderInformation
/// @brief Shader information class that not serve heap instance
/// but information to create heap instance.
///
class CDyShaderInformation final
{
public:
  CDyShaderInformation(const PDyShaderConstructionDescriptor& shaderConstructionDescriptor);

  CDyShaderInformation(const CDyShaderInformation&)            = delete;
  CDyShaderInformation& operator=(const CDyShaderInformation&) = delete;
  CDyShaderInformation(CDyShaderInformation&&)            = default;
  CDyShaderInformation& operator=(CDyShaderInformation&&) = default;
  ~CDyShaderInformation();

  ///
  /// @brief return immutable descriptor information reference.
  ///
  const PDyShaderConstructionDescriptor& GetInformation() const noexcept
  {
    return this->mShaderInformation;
  }

private:
  PDyShaderConstructionDescriptor mShaderInformation;

  ///
  /// @brief
  ///
  void __pfSetNextLevel(CDyShaderResource* ptr) const noexcept;
  mutable CDyShaderResource* mNextLevelPtr = nullptr;

  friend class CDyShaderResource;
  friend class MDyResource;
};

} /// ::dy namespace

#endif /// GUARD_DY_SHADER_INFORMATION_H