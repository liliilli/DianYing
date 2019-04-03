#ifndef GUARD_DY_CORE_RESOURCE_INFORMATION_FInformationShader_H
#define GUARD_DY_CORE_RESOURCE_INFORMATION_FInformationShader_H
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

#include <Dy/Core/Resource/Internal/XShaderTypes.h>

//!
//! Forward declaration
//!

namespace dy
{
struct PDyGLShaderInstanceMetaInfo;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

/// @class FInformationShader
/// @brief Shader information instance.
class FInformationShader final
{
public:
  using TShaderFragmentList = std::vector<PShaderFragmentInformation>;

  MDY_ONLY_MOVEABLE_PROPERTIES_DEFAULT(FInformationShader);

  /// @brief Construct shader information.
  FInformationShader(const PDyGLShaderInstanceMetaInfo& metaInfo, bool iIsInstantiable);
  ~FInformationShader() = default;

  /// @brief Get shader's specifier name.
  MDY_NODISCARD const std::string& GetSpecifierName() const noexcept;

  /// @brief Get valid shader's shader fragment list.
  MDY_NODISCARD const auto& GetShaderFragmentList() const noexcept
  {
    return this->mShaderFragmentList;
  }

  /// @brief Check this shader is instantiable (Instancing)
  MDY_NODISCARD bool IsInstantiable() const noexcept;

private:
  std::string mSpecifierName;
  TShaderFragmentList mShaderFragmentList = {};
  bool mIsInstantiable = false;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_INFORMATION_FInformationShader_H