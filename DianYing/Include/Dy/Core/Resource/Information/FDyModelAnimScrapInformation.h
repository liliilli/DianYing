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
#include <Dy/Helper/Type/DQuaternion.h>

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

  /// @brief Get interpolated scaling value.
  /// [0  ][1  ][2  ].....[N  ]__[0  ]
  /// |<-dt(0) :: v = (dt - 0)/(1 - 0); result = S0 * (1 - v) + S1 * v;
  ///                       |<-dt(N) When Looped
  ///                       L v = (dt - N) / (elapsed_time - N); result = SN * (1 - v) + S0 * v;
  ///                     |<-dt(N) When not looped, just return SN;
  MDY_NODISCARD DVector3 GetInterpolatedScaling(_MIN_ TF32 iElapsedTime, _MIN_ TU32 iBoneIndex, _MIN_ bool iIsLooped) const;
  /// @brief Get interpolated rotation value.
  MDY_NODISCARD DQuaternion GetInterpolatedRotation(_MIN_ TF32 iElapsedTime, _MIN_ TU32 iBoneIndex, _MIN_ bool iIsLooped) const;
  /// @brief Get interpolated position value.
  MDY_NODISCARD DVector3 GetInterpolatedPosition(_MIN_ TF32 iElapsedTime, _MIN_ TU32 iBoneIndex, _MIN_ bool iIsLooped) const;

  /// @brief Get reference of list of skeleton bone.
  MDY_NODISCARD const decltype(DDyAnimationSequence::mAnimationNodeList)& GetAnimNodeList() const noexcept;
  /// @brief Get rate scale.
  MDY_NODISCARD TF32 GetRateScale() const noexcept;

private:
  std::string           mSpecifierName = MDY_INITIALIZE_EMPTYSTR;
  DDyAnimationSequence  mAnimation = {}; 
  TF32                  mRateScale = 0.0f;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_INFORMATION_FDyModelAnimScrapInformation_H