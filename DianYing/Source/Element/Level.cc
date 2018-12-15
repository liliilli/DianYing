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
#include <Dy/Management/WorldManager.h>
#include <Dy/Management/IO/MetaInfoManager.h>

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

void FDyLevel::Initialize(_MIN_ const PDyLevelConstructMetaInfo& desc)
{
  // Lambda functions

  ///
  /// @brief  Create pawn instance and set fundamental properties.
  /// @param  objectInformation Information to create FDyPawn instance.
  ///
  static auto pCreateActorInstance = [this](_MIN_ const PDyObjectMetaInfo& objectInformation)
  {
    // Make FDyActor instance.
    auto instancePtr = std::make_unique<FDyActor>();
    MDY_CALL_ASSERT_SUCCESS(instancePtr->Initialize(objectInformation));

    // Update transform to reflect transform information.
    MDY_NOTUSED const auto& _ = instancePtr->GetTransform()->GetTransform();

    // Check activation flags and execute sub-routines of each components.
    instancePtr->pUpdateActivateFlagFromParent();
    if (objectInformation.mProperties.mInitialActivated == true) { instancePtr->Activate(); }

    // @TODO TEMPORARY.
    auto [it, result] = this->mActorMap.try_emplace(instancePtr->GetActorName(), std::move(instancePtr));
    MDY_ASSERT(result == true, "Unexpected error occured in inserting FDyActor to object map.");
  };

  // FunctionBody ∨

  this->mLevelName            = desc.mMetaCategory.mLevelName;
  // @TODO REMOVE THIS AS SOON AS IMPLEMENT HASH SAVE
  this->mLevelHashIdentifier  = hash::DyToCrc32Hash(this->mLevelName.c_str());
  this->mLevelBackgroundColor = desc.mMetaCategory.mLevelBackgroundColor;

  // Create object, FDyActor
  for (const auto& objectInformation : desc.mLevelObjectMetaInfoList)
  {
    const auto type = objectInformation->mObjectType;
    switch (type)
    {
    default: MDY_UNEXPECTED_BRANCH();    break;
    case EDyMetaObjectType::Actor:          pCreateActorInstance(*objectInformation); break;
    case EDyMetaObjectType::SceneScriptor:  MDY_NOT_IMPLEMENTED_ASSERT(); break;
    case EDyMetaObjectType::Object:         MDY_NOT_IMPLEMENTED_ASSERT(); break;
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
MDY_NOT_IMPLEMENTED_ASSERT();
return MDY_INITIALIZE_EMPTYSTR;
}

} /// ::dy namespace