#include <precompiled.h>
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
#include <Dy/Core/Resource/Type/ModelMesh/DDySkeletonBone.h>
#include <Dy/Helper/Library/HelperJson.h>

namespace dy
{
  
/// @brief Serialization function. `mPtrAiNode` will be neglected.
void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const DDySkeletonBone& p)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

/// @brief Deserialization function.
void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DDySkeletonBone& p)
{
  DyJsonGetValueFromTo(j, "Specifier", p.mSpecifier);
  DyJsonGetValueFromTo(j, "ParentBoneId", p.mParentSkeletonBoneIndex);
  DyJsonGetValueFromTo(j, "LocalTf",  p.mLocalTransform);
  DyJsonGetValueFromTo(j, "GlobalTf", p.mGlobalTransform);
  DyJsonGetValueFromTo(j, "OffsetMt", p.mOffsetMatrix);
}

} /// ::dy namespace
