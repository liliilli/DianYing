#ifndef GUARD_DY_CORE_RESOURCE_INFORMATION_FInformationModelAnimScrap_H
#define GUARD_DY_CORE_RESOURCE_INFORMATION_FInformationModelAnimScrap_H
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

#include <Dy/Core/Resource/Type/ModelMesh/DAnimationSequence.h>
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

/// @class FInformationModelAnimScrap
/// @brief Model animation scrap information instance.
class FInformationModelAnimScrap final
{
public:
  MDY_ONLY_MOVEABLE_PROPERTIES_DEFAULT(FInformationModelAnimScrap);

  /// @brief Construct model animation scrap information.
  FInformationModelAnimScrap(_MIN_ const PDyModelAnimInstanceMetaInfo& metaInfo);
  ~FInformationModelAnimScrap() = default;

  /// @brief Get model specifier name.
  MDY_NODISCARD const std::string& GetSpecifierName() const noexcept;

  /// @brief Get interpolated scaling value.
  /// [0  ][1  ][2  ].....[N  ]__[0  ]
  /// |<-dt(0) :: v = (dt - 0)/(1 - 0); result = S0 * (1 - v) + S1 * v;
  ///                       |<-dt(N) When Looped
  ///                       L v = (dt - N) / (elapsed_time - N); result = SN * (1 - v) + S0 * v;
  ///                     |<-dt(N) When not looped, just return SN;
  MDY_NODISCARD DVec3 GetInterpolatedScaling(_MIN_ TF32 iElapsedTime, _MIN_ TU32 iBoneIndex, _MIN_ bool iIsLooped) const;
  /// @brief Get interpolated rotation value.
  MDY_NODISCARD DQuat GetInterpolatedRotation(_MIN_ TF32 iElapsedTime, _MIN_ TU32 iBoneIndex, _MIN_ bool iIsLooped) const;
  /// @brief Get interpolated position value.
  MDY_NODISCARD DVec3 GetInterpolatedPosition(_MIN_ TF32 iElapsedTime, _MIN_ TU32 iBoneIndex, _MIN_ bool iIsLooped) const;

  /// @brief Get reference of list of skeleton bone.
  MDY_NODISCARD const decltype(DAnimationSequence::mAnimationNodeList)& GetAnimNodeList() const noexcept;
  /// @brief Get rate scale.
  MDY_NODISCARD TF32 GetRateScale() const noexcept;

private:
  std::string           mSpecifierName = MDY_INITIALIZE_EMPTYSTR;
  DAnimationSequence  mAnimation = {}; 
  TF32                  mRateScale = 0.0f;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_INFORMATION_FInformationModelAnimScrap_H