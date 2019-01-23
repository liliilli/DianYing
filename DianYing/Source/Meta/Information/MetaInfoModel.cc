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
#include <Dy/Meta/Information/MetaInfoModel.h>
#include <Dy/Helper/Library/HelperJson.h>

//!
//! Forward declaration
//!

namespace
{



} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy
{

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyModelInstanceMetaInfo& p)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyModelInstanceMetaInfo& p)
{
  /* Template (2019-01-21)
   * "TestModel": {
       "ExternalPath": "./data/TestResource/nanosuit/nanosuit.blend",
       "SkeletonName": "",
       "Flags": { "IsUsingSkeleton": false, "IsLoadMaterial": false },
       "Transform": {
         "Position": { "X": 0, "Y": 0, "Z": 0 },
         "Rotation": { "X": 0, "Y": 0, "Z": 0 },
         "Scale": { "X": 1, "Y": 1, "Z": 1 }
       }
     }
   */

  /* Template (2019-01-23)
   * "TestModel": {
   *   "ExternalPath": "...",
   *   "IsCompressed": false
   * }
   */
  
  p.mSourceType         = EDyResourceSource::External;
  p.mExternalPath       = DyJsonGetValueFrom<std::string>(j, "ExternalPath");
  p.mIsCompressed       = DyJsonGetValueFrom<bool>(j, "IsCompressed");
#ifdef false
  p.mSkeletonSpecifier  = DyJsonGetValueFrom<std::string>(j, "SkeletonName");
  p.mModelFlags         = DyJsonGetValueFrom<PDyModelInstanceMetaInfo::DFlags>(j, "Flags");
  p.mInitialTransform   = DyJsonGetValueFrom<PDyModelInstanceMetaInfo::DInitialTransform>(j, "Transform");
#endif
}

#ifdef false
void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyModelInstanceMetaInfo::DFlags& p)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyModelInstanceMetaInfo::DFlags& p)
{
  p.mIsUsingSkeleton = DyJsonGetValueFrom<bool>(j, "IsUsingSkeleton");
  p.mIsUsingMaterial = DyJsonGetValueFrom<bool>(j, "IsLoadMaterial");
}

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyModelInstanceMetaInfo::DInitialTransform& p)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyModelInstanceMetaInfo::DInitialTransform& p)
{
  p.mPosition = DyJsonGetValueFrom<DDyVector3>(j, "Position");
  p.mRotation = DyJsonGetValueFrom<DDyVector3>(j, "Rotation");
  p.mScale    = DyJsonGetValueFrom<DDyVector3>(j, "Scale");
}
#endif

} /// ::dy namespace