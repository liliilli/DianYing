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
          { "Specifier": "nanosuit_Visor", "Material": "dyBtMtCheckerWorldPos", "IsInstancing": true }
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
  json::GetValueFromTo(j, "Mesh",     p.mMeshList);
  json::GetValueFromTo(j, "Skeleton", p.mSkeleton);
  json::GetValueFromTo(j, "Transform", p.mTransform);
}

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyModelInstanceMetaInfo::DMesh& p)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyModelInstanceMetaInfo::DMesh& p)
{
  json::GetValueFromTo(j, "Specifier",  p.mMeshSpecifier);
  json::GetValueFromTo(j, "Material",   p.mMaterialSpecifier);
  if (json::HasJsonKey(j, "IsInstancing") == true)
  {
    json::GetValueFromTo(j, "IsInstancing", p.mInstanced);
  }
  else
  {
    p.mInstanced = false;
  }
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
  json::GetValueFromTo(j, "IsUsingSkeleton",    p.mIsUsingSkeleton);
  json::GetValueFromTo(j, "SkeletonSpecifier",  p.mSkeletonSpecifier);
}

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyModelInstanceMetaInfo::DTransform& p)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyModelInstanceMetaInfo::DTransform& p)
{
  json::GetValueFromTo(j, "Position",   p.mPosition);
  json::GetValueFromTo(j, "Rotation",   p.mRotation);
  json::GetValueFromTo(j, "Scale",      p.mScale);
}

} /// ::dy namespace