#ifndef GUARD_DY_CORE_RESOURCE_TYPE_MODELMESH_DDyAnimationSequence_H
#define GUARD_DY_CORE_RESOURCE_TYPE_MODELMESH_DDyAnimationSequence_H
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

#include <Dy/Helper/Type/Vector3.h>

namespace dy
{

struct DDyAnimationSequence final
{
  struct DHeader final
  {
    float     mDurationSecond = 0.0f;
    float     mTickSecond     = 0.0f;
    float     mDurationTicks  = 0;
    float     mTickPerSecond  = 0;
    unsigned  mNumChannels    = 0;
  };

  struct DAnimNode final
  {
    struct DTranslate final
    {
      float       mStartSecond  = 0.0f;
      DDyVector3  mTranslate    = {};
    };
    struct DRotation final
    {
      float mStartSecond = 0.0f;
      float mW{};
      float mX{};
      float mY{};
      float mZ{};
    };
    struct DScale final
    {
      float mStartSecond  = 0.0f;  
      DDyVector3 mScale   = {};
    };

    unsigned  mSkeletonBoneId = 0;
    std::vector<DTranslate> mPositionList = {};
    std::vector<DRotation>  mRotationList = {};
    std::vector<DScale>     mScaleList    = {};
  };

  DHeader                 mAnimationHeader    = {};
  std::vector<DAnimNode>  mAnimationNodeList  = {};
};

} /// ::dy namespace

#endif // !GUARD_DY_CORE_RESOURCE_TYPE_MODELMESH_DDyAnimationSequence_H
