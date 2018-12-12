#ifndef GUARD_DY_CORE_COMPONENT_RESOURCE_MODEL_RESOURCE_H
#define GUARD_DY_CORE_COMPONENT_RESOURCE_MODEL_RESOURCE_H
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

#include <Dy/Core/Resource/Resource_Deprecated/SubmeshResource_Deprecated.h>
#include <Dy/Core/Resource/Internal/MaterialType.h>

//!
//! Forward declaration
//!

struct aiScene;
struct aiNode;

namespace dy
{
  struct DMoeBoneNodeInformation;
  class DDyModelInformation_Deprecated;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

///
/// @class CDyModelResource
/// @brief Model resource instance for storing and managing mesh nonsafe heap resource
/// and other properties.
///
class CDyModelResource
{
public:
  CDyModelResource() = default;
  CDyModelResource(const CDyModelResource&)             = delete;
  CDyModelResource(CDyModelResource&&)                  = default;
  CDyModelResource& operator=(const CDyModelResource&)  = delete;
  CDyModelResource& operator=(CDyModelResource&&)       = default;
  ~CDyModelResource();

  ///
  /// @brief Check this model resource is able to animated.
  ///
  [[nodiscard]]
  FORCEINLINE bool IsEnabledModelAnimated() const noexcept
  {
    return this->mIsEnabledModelSkeletalAnimation;
  }

  ///
  /// @brief Get valid submesh resource, not modifiable.
  ///
  [[nodiscard]]
  const std::vector<std::unique_ptr<CDySubmeshResource_Deprecated>>& GetSubmeshResources() const noexcept
  {
    return this->mMeshResource;
  }

  ///
  /// @brief Update bone transforms by elapsedTime.
  ///
  void UpdateBoneAnimationTransformList(float elapsedTime);

  ///
  /// @brief Get model animation transform matrix list for moving animation of model resource.
  ///
  [[nodiscard]]
  const std::vector<DDyGeometryBoneInformation>&
  GetModelAnimationTransformMatrixList() const noexcept;

private:
  ///
  /// @brief Initialize model resource with model information instance.
  ///
  [[nodiscard]]
  EDySuccess pInitializeModelResource(const DDyModelInformation_Deprecated& modelInformation);

  ///
  /// @brief Read node hierarchy
  ///
  void pReadNodeHierarchy(float animationElapsedTime, DDyModelInformation_Deprecated& modelInfo,
      const DMoeBoneNodeInformation& boneNode, const DDyMatrix4x4& parentTransform);

  std::vector<std::unique_ptr<CDySubmeshResource_Deprecated>>  mMeshResource                     = {};
  bool                                              mIsEnabledModelSkeletalAnimation  = false;
  TI32                                              tempAnimationNumber = 0;

  //!
  //! Level pointers binding
  //!

  FORCEINLINE void __pfSetModelInformationLink(NotNull<DDyModelInformation_Deprecated*> ptr) const noexcept
  {
    this->__mLinkedModelInformationPtr = ptr;
  }
  FORCEINLINE void __pfResetModelInformationLink() const noexcept
  {
    this->__mLinkedModelInformationPtr = nullptr;
  }

  MDY_TRANSIENT DDyModelInformation_Deprecated*  __mLinkedModelInformationPtr = nullptr;

  friend class DDyModelInformation_Deprecated;
  friend class MDyIOResource_Deprecated;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_COMPONENT_RESOURCE_MODEL_RESOURCE_H