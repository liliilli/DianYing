#ifndef GUARD_DY_CORE_COMPONENT_INTERNAL_MODEL_TYPE_H
#define GUARD_DY_CORE_COMPONENT_INTERNAL_MODEL_TYPE_H
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

#include <string>

#include <Dy/Core/Resource/Internal/GeometryType.h>
#include <Dy/Element/Interface/IDyToString.h>
#include <Dy/Helper/Type/DQuaternion.h>

namespace dy
{

struct DMoeTempAnimationChannel;

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

    std::vector<DQuaternion>  mRotationKeys = {};
    std::vector<DVector3>     mPositionKeys = {};
    std::vector<DVector3>     mScalingKeys  = {};

    std::vector<float>          mRotationTime = {};
    std::vector<float>          mPositionTime = {};
    std::vector<float>          mScalingTime  = {};
  };

  std::string mName           = "";
  float       mDuration       = MDY_INITIALIZE_DEFUINT;
  float       mTickPerSecond  = MDY_INITIALIZE_DEFINT;

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
  DMatrix4x4                          mNodeTransform  = DMatrix4x4::IdentityMatrix();
  DMatrix4x4                          mOffsetMatrix   = DMatrix4x4::IdentityMatrix();
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_COMPONENT_INTERNAL_MODEL_TYPE_H