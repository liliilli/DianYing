#ifndef GUARD_DY_CORE_RESOURCE_INFORMATION_FDYMESHINFORMATION_H
#define GUARD_DY_CORE_RESOURCE_INFORMATION_FDYMESHINFORMATION_H
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

#include <Dy/Core/Resource/Internal/GeometryType.h>

//!
//! Forward declaration
//!

namespace dy
{
struct PDyModelInstanceMetaInfo;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{
///
/// @class FDyModelInformation
/// @brief Model information instance.
///
class FDyModelInformation final
{
public:
  MDY_ONLY_MOVEABLE_PROPERTIES_DEFAULT(FDyModelInformation);

  /// @brief Construct model information.
  FDyModelInformation(_MIN_ const PDyModelInstanceMetaInfo& metaInfo);
  ~FDyModelInformation() = default;

  /// @brief Get model specifier name.
  FORCEINLINE MDY_NODISCARD const std::string& GetSpecifierName() const noexcept
  {
    return this->mSpecifierName;
  }

  /// @brief Get submesh information list.
  MDY_NODISCARD const auto& GetMeshInformationList() const noexcept
  {
    return this->mMeshInformations;
  }

private:
  std::string mSpecifierName = MDY_INITIALIZE_EMPTYSTR;
  std::vector<PDySubmeshInformationDescriptor> mMeshInformations;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_INFORMATION_FDYMESHINFORMATION_H