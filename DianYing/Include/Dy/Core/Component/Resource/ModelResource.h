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

#include <Dy/Core/Component/Resource/SubmeshResource.h>

//!
//! Forward declaration
//!

struct aiScene;
struct aiNode;

namespace dy
{
class DDyModelInformation;
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
  FORCEINLINE bool IsEnabledModelAnimated() const noexcept
  {
    return this->mIsEnabledModelSkeletalAnimation;
  }

  ///
  /// @brief Get submesh resource, not modifiable.
  ///
  const std::vector<std::unique_ptr<CDySubmeshResource>>& GetSubmeshResources() const noexcept;

  ///
  /// @brief
  ///
  void GetBoneTransformLists(float runningTime, std::vector<DDyMatrix4x4>& transforms);

  ///
  /// @brief
  ///
  void SetBoneTransformLists(const std::vector<DDyMatrix4x4>& transforms);

  ///
  /// @brief
  ///
  FORCEINLINE const auto& GetModelAnimationTransformMatrixList() const noexcept
  {
    return this->mOverallModelAnimationMatrix;
  }

private:
  ///
  /// @brief Initialize model resource with model information instance.
  ///
  [[nodiscard]] EDySuccess pInitializeModelResource(const DDyModelInformation& modelInformation);

  ///
  /// @brief
  ///
  void pReadNodeHierarchy(float, const aiNode&, DDyModelInformation& modelInfo, const DDyMatrix4x4&);

  std::vector<std::unique_ptr<CDySubmeshResource>>  mMeshResource                     = {};
  std::vector<DDyMatrix4x4>                         mOverallModelAnimationMatrix      = {};
  bool                                              mIsEnabledModelSkeletalAnimation  = false;

  //!
  //! Level pointers binding
  //!

  template <typename TType>
  using TBindPtrMap = std::unordered_map<TType*, TType*>;

  void __pfLinkModelInformationPtr(DDyModelInformation* ptr) const noexcept
  {
    this->__mLinkedModelInformationPtr = ptr;
  }
  mutable DDyModelInformation*  __mLinkedModelInformationPtr = nullptr;

  friend class DDyModelInformation;
  friend class MDyHeapResource;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_COMPONENT_RESOURCE_MODEL_RESOURCE_H