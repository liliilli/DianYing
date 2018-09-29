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
#include <Dy/Element/Pawn.h>
#include <Dy/Management/WorldManager.h>
#include <Dy/Management/MetaInfoManager.h>

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
  ///
  /// @brief  Create pawn instance and set fundamental properties.
  /// @param  objectInformation Information to create FDyPawn instance.
  ///
  static auto pCreatePawn = [&](const DDyObjectInformation& objectInformation)
  {
    auto instancePtr = std::make_unique<FDyPawn>();
    MDY_CALL_ASSERT_SUCCESS(instancePtr->Initialize(objectInformation));

    // @TODO IMPLEMENT PARENT TRANSFORMATION RELOCATION MECHANISM
    if (objectInformation.mParentMetaIndex != -1)
    {
#ifdef false
      PHITOS_NOT_IMPLEMENTED_ASSERT();
      instancePtr->SetParent();
#endif
    }
    else
    {
      // Insert created ptr into actor map of this scene. (root)
      auto[it, result]    = this->mActorMap.try_emplace(objectInformation.mName, std::move(instancePtr));

      // Insert inserted FDyPawn raw pointer instance to script update list.
      FDyPawn* pawnRawPtr = static_cast<FDyPawn*>(it->second.get());
      const auto id       = MDyWorld::GetInstance().pfEnrollActivePawn(DyMakeNotNull(pawnRawPtr));
      pawnRawPtr->pfSetListIndex(id);

      if (!result) { PHITOS_UNEXPECTED_BRANCH(); }
    }
  };

  ///
  /// @brief  Create FDyDirectionalLight instance and set fundamental properties.
  /// @param  objectInformation Information to create FDyDirectionalLight instance.
  ///
  static auto pCreateDirectionalLight = [&](const DDyObjectInformation& objectInformation)
  {
    MDY_LOG_CRITICAL("EDyFDyObjectType::FDyDirectionalLight: NOT IMPLEMENTED");
  };

  // Body
  this->mLevelName = desc.mLevelName;
  this->mLevelHashIdentifier  = hash::DyToCrc32Hash(this->mLevelName.c_str());
  this->mLevelBackgroundColor = desc.mLevelBackgroundColor;

  // Create objects
  for (const auto& objectInformation : desc.mLevelObjectInformations)
  {
    const auto type = objectInformation.mType;
    switch (type)
    {
    case EDyFDyObjectType::FDyPawn:                 pCreatePawn(objectInformation); break;
    case EDyFDyObjectType::FDyDirectionalLight:     pCreateDirectionalLight(objectInformation); break;
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
  for (auto& [name, actor] : this->mActorMap)
  {
    if (!actor) continue;
    else
    {
      const auto flag = actor->Release();
      if (flag == DY_FAILURE)
      {
        MDY_LOG_WARNING("{} | Release function is failed. | Actor name : {}", "FDyLevel::Release", actor->GetActorName());
      }

      MDyWorld::GetInstance().pfMoveActorToGc(DyMakeNotNull(actor.release()));
    }
  }

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