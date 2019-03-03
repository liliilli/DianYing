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
#include <nlohmann/json.hpp>
#include "Dy/Builtin/Material/BtMtCheckerWorldPos.h"

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
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyModelInstanceMetaInfo& p)
{
  /* Template (2019-01-24)
   *{
      "Nanosuit":
      {
        "Mesh": 
        [
          { "Specifier": "nanosuit_Arms", "Material": "dyBtMtCheckerWorldPos" },
          { "Specifier": "nanosuit_Body", "Material": "dyBtMtCheckerWorldPos" },
          { "Specifier": "nanosuit_Hands", "Material": "dyBtMtCheckerWorldPos" },
          { "Specifier": "nanosuit_Helmet", "Material": "dyBtMtCheckerWorldPos" },
          { "Specifier": "nanosuit_Legs", "Material": "dyBtMtCheckerWorldPos" },
          { "Specifier": "nanosuit_Lights", "Material": "dyBtMtCheckerWorldPos" },
          { "Specifier": "nanosuit_Visor", "Material": "dyBtMtCheckerWorldPos" }
        ],
        "Skeleton":
        {
          "IsUsingSkeleton": false,
          "SkeletonSpecifier": ""
        }
      }
    }
   */ 

  p.mSourceType = EDyResourceSource::External;
  DyJsonGetValueFromTo(j, "Mesh",     p.mMeshList);
  DyJsonGetValueFromTo(j, "Skeleton", p.mSkeleton);
  DyJsonGetValueFromTo(j, "Transform", p.mTransform);
}

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyModelInstanceMetaInfo::DMesh& p)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyModelInstanceMetaInfo::DMesh& p)
{
  DyJsonGetValueFromTo(j, "Specifier",  p.mMeshSpecifier);
  DyJsonGetValueFromTo(j, "Material",   p.mMaterialSpecifier);
  // 
  if (p.mMaterialSpecifier.empty() == true)
  {
    p.mMaterialSpecifier = (builtin::FDyBtMtCheckerWorldPos::sName);
  }
}

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyModelInstanceMetaInfo::DSkeleton& p)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyModelInstanceMetaInfo::DSkeleton& p)
{
  DyJsonGetValueFromTo(j, "IsUsingSkeleton",    p.mIsUsingSkeleton);
  DyJsonGetValueFromTo(j, "SkeletonSpecifier",  p.mSkeletonSpecifier);
}

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyModelInstanceMetaInfo::DTransform& p)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyModelInstanceMetaInfo::DTransform& p)
{
  DyJsonGetValueFromTo(j, "Position",   p.mPosition);
  DyJsonGetValueFromTo(j, "Rotation",   p.mRotation);
  DyJsonGetValueFromTo(j, "Scale",      p.mScale);
}

} /// ::dy namespace