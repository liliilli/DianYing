#pragma once
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

/// @struct DDyAnimationSequence
/// @brief Animation sequence information type for exporting model with skinned animation.
struct DDyAnimationSequence final
{
  /// @struct DHeader
  /// @brief Overall information of animation information.
  struct DHeader final
  {
    /// @brief Duration of animation that has unit as second.
    float     mDurationSecond = 0.0f;
    /// @brief Second per tick.
    float     mTickSecond     = 0.0f;
    /// @brief Duration of animation that has unit as tick.
    float     mDurationTicks  = 0;
    /// @brief Count of ticks per second.
    float     mTickPerSecond  = 0;
    /// @brief Animation node channel's list count.
    unsigned  mNumChannels    = 0;
  };

  /// @struct DAnimNode
  /// @brief Animation node for each animation channels.
  /// Each animation channel will be attached to each skeleton node.
  struct DAnimNode final
  {
    struct DTranslate final
    {
      float      mStartSecond = 0.0f;
      DDyVector3 mTranslate   = {};
    };
    struct DRotation final
    {
      float mStartSecond = 0.0f;
      float mW, mX, mY, mZ{};
    };
    struct DScale final
    {
      float mStartSecond  = 0.0f;  
      DDyVector3 mScale   = {};
    };

    int mSkeletonNodeId = -1; // If Skeleton node id is -1, neglected.
    std::vector<DTranslate> mPositionList = {};
    std::vector<DRotation>  mRotationList = {};
    std::vector<DScale>     mScaleList    = {};
  };

  DHeader                 mAnimationHeader    = {};
  std::vector<DAnimNode>  mAnimationNodeList  = {};
};
