#ifndef GUARD_DY_CORE_RESOURCE_RESOURCE_FDYSHADERRESOURCE_H
#define GUARD_DY_CORE_RESOURCE_RESOURCE_FDYSHADERRESOURCE_H
#include "Dy/Meta/Type/EDyResourceTypes.h"
#include <complex.h>
#include <vector>
#include "Dy/Core/Resource/Information/FDyShaderInformation.h"
#include <complex.h>

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
class FDyShaderInformation;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

///
/// @class FDyShaderResource
/// @brief Shader resource class that serve heap instance.
///
class FDyShaderResource final
{
public:
  /// @warning input parameter must be atomic!
  FDyShaderResource(_MIN_ const FDyShaderInformation& information);
  ~FDyShaderResource();

  /// @brief Get specifier name of valid shader resource.
  MDY_NODISCARD const std::string& GetSpecifierName() const noexcept
  {
    return this->mSpecifierName;
  }

  /// @brief Get internal shader program id.
  MDY_NODISCARD TU32 GetShaderProgramId() const noexcept;

  /// @brief Turn on shader program.
  void UseShader() const noexcept;

  /// @brief Turn off shader program.
  void DisuseShader() const noexcept;

private:
  /// @brief Create shader fragment list and return.
  MDY_NODISCARD std::optional<TFragmentList> pCreateShaderFragments(_MIN_ const FDyShaderInformation::TShaderFragmentList& vector);
  /// @brief Create shader program on gpu and return id when it's succeeded.
  MDY_NODISCARD std::optional<TU32> pInitializeShaderProgram(_MIN_ const TFragmentList& fragmentList);
  /// @brief Delete shader fragment list from gpu memory.
  void pDeleteShaderFragments(_MIN_ const TFragmentList& fragmentList);

  std::string mSpecifierName    = MDY_INITIALIZE_EMPTYSTR;
  TU32        mShaderProgramId;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_RESOURCE_FDYSHADERRESOURCE_H