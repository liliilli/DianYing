#ifndef GUARD_DY_CORE_RESOURCE_INFORMATION_FDyModelAnimScrapInformation_H
#define GUARD_DY_CORE_RESOURCE_INFORMATION_FDyModelAnimScrapInformation_H
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

#include <Dy/Core/Resource/Type/ModelMesh/DDyAnimationSequence.h>

//!
//! Forward declaration
//!

namespace dy
{
struct PDyModelAnimInstanceMetaInfo;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

///
/// @class FDyModelAnimScrapInformation
/// @brief Model animation scrap information instance.
///
class FDyModelAnimScrapInformation final
{
public:
  MDY_ONLY_MOVEABLE_PROPERTIES_DEFAULT(FDyModelAnimScrapInformation);

  /// @brief Construct model animation scrap information.
  FDyModelAnimScrapInformation(_MIN_ const PDyModelAnimInstanceMetaInfo& metaInfo);
  ~FDyModelAnimScrapInformation() = default;

  /// @brief Get model specifier name.
  MDY_NODISCARD const std::string& GetSpecifierName() const noexcept;

private:
  std::string           mSpecifierName = MDY_INITIALIZE_EMPTYSTR;
  DDyAnimationSequence  mAnimation = {}; 
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_INFORMATION_FDyModelAnimScrapInformation_H