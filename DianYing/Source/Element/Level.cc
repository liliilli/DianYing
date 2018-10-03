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

void FDyLevel::Initialize(_MIN_ const PDyLevelConstructDescriptor& desc)
{
  ///
  /// @brief  Create pawn instance and set fundamental properties.
  /// @param  objectInformation Information to create FDyPawn instance.
  ///
  static auto pCreateActorInstance = [&](_MIN_ const DDyObjectInformation& objectInformation)
  {
    auto instancePtr = std::make_unique<FDyActor>();
    MDY_CALL_ASSERT_SUCCESS(instancePtr->Initialize(objectInformation));

    // @TODO IMPLEMENT PARENT TRANSFORMATION RELOCATION MECHANISM
    if (objectInformation.mParentHashValue.empty() == false)
    {
#ifdef false
      PHITOS_NOT_IMPLEMENTED_ASSERT();
      instancePtr->SetParent();
#endif
    }
  };

  // FunctionBody ∨

  this->mLevelName            = desc.mLevelName;
  // @TODO REMOVE THIS AS SOON AS IMPLEMENT HASH SAVE
  this->mLevelHashIdentifier  = hash::DyToCrc32Hash(this->mLevelName.c_str());
  this->mLevelBackgroundColor = desc.mLevelBackgroundColor;

  // Create object, FDyActor
  for (const auto& objectInformation : desc.mLevelObjectInformations)
  {
    const auto type = objectInformation.mObjectType;
    switch (type)
    {
    default: PHITOS_UNEXPECTED_BRANCH();    break;
    case EDyMetaObjectType::Actor:          pCreateActorInstance(objectInformation); break;
    case EDyMetaObjectType::SceneScriptor:  PHITOS_NOT_IMPLEMENTED_ASSERT(); break;
    case EDyMetaObjectType::Object:         PHITOS_NOT_IMPLEMENTED_ASSERT(); break;
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
return MDY_INITILAIZE_EMPTYSTR;
}

} /// ::dy namespace