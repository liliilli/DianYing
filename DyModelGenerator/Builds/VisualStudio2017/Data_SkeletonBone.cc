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

/// Header file
#include "Data_SkeletonBone.h"
#include <nlohmann/json.hpp>

void to_json(nlohmann::json& j, const DSkeletonBone& p)
{
  j = nlohmann::json
  {
    {"Specifier",     p.mSpecifier},
    {"ParentBoneId",  p.mParentSkeletonNodeIndex},
    {"LocalTf",       p.mLocalTransform},
    {"BoneOffsetId",  p.mBoneOffsetId}
  };
}

void from_json(const nlohmann::json& j, DSkeletonBone& p)
{
  (void)j;
  (void)p;
}

void to_json(nlohmann::json& j, const DDyBoneOffset& p)
{
  j = nlohmann::json
  {
    {"Specifier", p.mBoneName},
    {"SkeletonNodeId", p.mIndexSkeletonNode},
    {"OffsetMatrix", p.mBoneOffsetMatrix},
  };
}

void to_json(nlohmann::json& j, const DDySkeleton& p)
{
  j = nlohmann::json
  {
    {"InverseTransform", p.mSkeletonRootInverseTransform},
    {"SkeletonNodeList", p.mExportedSkeleton},
    {"BoneOffsetList", p.mBoneOffsetList}
  };
}
