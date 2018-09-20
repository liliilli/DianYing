#ifndef GUARD_DY_CORE_COMPONENT_INTERNAL_MODEL_TYPE_H
#define GUARD_DY_CORE_COMPONENT_INTERNAL_MODEL_TYPE_H
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

#include <string>
#include <Dy/Core/Component/Interface/IDyToString.h>
#include "Dy/Helper/Type/Quaternion.h"

namespace dy
{

struct DMoeTempAnimationChannel;

///
/// @struct PDyModelConstructionDescriptor
/// @brief Model information construction descriptor
///
struct PDyModelConstructionDescriptor final : public IDyToString
{
  std::string                           mModelName        = "";
  std::string                           mModelPath        = "";

  [[nodiscard]]
  std::string ToString() override final
  {
    return fmt::format(
R"dy(PDyModelConstructionDescriptor
Model Name : {}
Model Path : {}
)dy", this->mModelName, this->mModelPath);
  };
};

///
/// @struct DMoeAnimationInformation
/// @brief
///
struct DMoeAnimationInformation final
{
  ///
  /// @struct DAnimChannel
  /// @brief
  ///
  struct DAnimChannel final
  {
    // Anim channel name == bondId name == node name.
    std::string                 mName         = "";

    std::vector<DDyQuaternion>  mRotationKeys = {};
    std::vector<DDyVector3>     mPositionKeys = {};
    std::vector<DDyVector3>     mScalingKeys  = {};

    std::vector<float>          mRotationTime = {};
    std::vector<float>          mPositionTime = {};
    std::vector<float>          mScalingTime  = {};
  };

  std::string mName           = "";
  float       mDuration       = MDY_NOT_INITIALIZED_0;
  float       mTickPerSecond  = MDY_NOT_INITIALIZED_M1;

  std::vector<DAnimChannel> mAnimationChannels      = {};
};

///
/// @struct DMoeBoneNodeInformation
/// @brief
///
struct DMoeBoneNodeInformation final
{
  DMoeBoneNodeInformation*              mParentNodePtr  = nullptr;

  std::string                           mName           = "";
  std::vector<DMoeBoneNodeInformation>  mChildrenNodes  = {};
  DDyMatrix4x4                          mNodeTransform  = DDyMatrix4x4::IdentityMatrix();
  DDyMatrix4x4                          mOffsetMatrix   = DDyMatrix4x4::IdentityMatrix();
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_COMPONENT_INTERNAL_MODEL_TYPE_H