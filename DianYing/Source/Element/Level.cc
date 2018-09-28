#include <precompiled.h>
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

/// Header file
#include <Dy/Element/Level.h>
#include <Dy/Helper/HashCompileCrc32.h>
#include "Dy/Element/Pawn.h"

//!
//! Local translation unit function & data
//!

namespace
{

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy
{

void FDyLevel::Initialize(const PDyLevelConstructDescriptor& desc)
{
  this->mLevelName            = desc.mLevelName;
  this->mLevelHashIdentifier  = hash::DyToCrc32Hash(this->mLevelName.c_str());
  this->mLevelBackgroundColor = desc.mLevelBackgroundColor;

  // Create objects
  for (const auto& objectInformation : desc.mLevelObjectInformations)
  {
    const auto type = objectInformation.mType;
    switch (type)
    {
    case EDyFDyObjectType::FDyPawn:
      {
        auto instancePtr = std::make_unique<CDyPawn>();
        MDY_CALL_ASSERT_SUCCESS(instancePtr->Initialize(objectInformation));
        // @TODO IMPLEMENT PARENT TRANSFORMATION RELOCATION MECHANISM
        if (objectInformation.mParentMetaIndex != -1)
        {
#ifdef false
          instancePtr->SetParent();
#endif
        }
        else
        {
          auto [it, result] = this->mActorList.try_emplace(objectInformation.mName, std::move(instancePtr));
          if (result == false) { PHITOS_UNEXPECTED_BRANCH(); }
        }
      } break;
    case EDyFDyObjectType::FDyDirectionalLight:
      {
        MDY_LOG_CRITICAL("EDyFDyObjectType::FDyDirectionalLight: NOT IMPLEMENTED");
      } break;
    case EDyFDyObjectType::FDyPostprocessBlock:
    case EDyFDyObjectType::FDyPointLight:
    case EDyFDyObjectType::FDySpotLight:
    case EDyFDyObjectType::FDyObject:
    case EDyFDyObjectType::FDySceneScriptableObject:
    case EDyFDyObjectType::FDyCamera:
    case EDyFDyObjectType::FDySound:
    case EDyFDyObjectType::FDySoundListener:
    case EDyFDyObjectType::Error:
      MDY_LOG_CRITICAL("NOT IMPLEMENTED");
      PHITOS_NOT_IMPLEMENTED_ASSERT();
      break;
    }
  }

  MDY_LOG_INFO("{} | Initialize level context. | Level name : {}", "FDyLevel::Initialize()", this->mLevelName);
  this->mInitialized = true;
}

void FDyLevel::Release()
{
  MDY_LOG_INFO("{} | Release level context. | Level name : {}", "FDyLevel::Release()", this->mLevelName);
  this->mInitialized = false;
}

void FDyLevel::Update(float dt)
{
  if (!this->mInitialized) { return; }


}

std::string FDyLevel::ToString()
{
PHITOS_NOT_IMPLEMENTED_ASSERT();
return MDY_NOT_INITILAIZED_STR;
}

} /// ::dy namespace