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

FDyLevel::FDyLevel(_MIN_ const PDyLevelConstructMetaInfo& desc)
{
  /// @brief  Create pawn instance and set fundamental properties.
  /// @param  objectInformation Information to create FDyPawn instance.
  static auto pCreateActorInstance = [](_MIN_ const PDyObjectMetaInfo& objectInformation)
  {
    // Make FDyActor instance.
    auto instancePtr = std::make_unique<FDyActor>(objectInformation);

    // Check activation flags and execute sub-routines of each components.
    instancePtr->pUpdateActivateFlagFromParent();
    if (objectInformation.mProperties.mInitialActivated == true) { instancePtr->Activate(); }
    return instancePtr;
  };

  // FunctionBody ∨

  this->mLevelName            = desc.mMetaCategory.mLevelName;
  this->mLevelBackgroundColor = desc.mMetaCategory.mLevelBackgroundColor;

  // Create object, FDyActor
  for (const auto& objectInformation : desc.mLevelObjectMetaInfoList)
  {
    switch (objectInformation->mObjectType)
    {
    case EDyMetaObjectType::Actor:
    {
      auto instancePtr  = pCreateActorInstance(*objectInformation);
      auto [it, result] = this->mActorMap.try_emplace(instancePtr->GetActorName(), std::move(instancePtr));
      MDY_ASSERT(result == true, "Unexpected error occured in inserting FDyActor to object map.");
    } break;
    case EDyMetaObjectType::SceneScriptor:  MDY_NOT_IMPLEMENTED_ASSERT(); break;
    case EDyMetaObjectType::Object:         MDY_NOT_IMPLEMENTED_ASSERT(); break;
    default: MDY_UNEXPECTED_BRANCH(); break;
    }
  }

  MDY_LOG_INFO("{} | Initialize level context. | Level name : {}", "FDyLevel::Initialize()", this->mLevelName);
  this->mInitialized = true;
}

FDyLevel::~FDyLevel()
{
  MDY_LOG_INFO("{} | Release level context. | Level name : {}", "FDyLevel::Release()", this->mLevelName);
  for (auto& [name, actor] : this->mActorMap)
  {
    if (MDY_CHECK_ISEMPTY(actor)) { continue; }
    MDyWorld::GetInstance().pfMoveActorToGc(DyMakeNotNull(actor.release()));
  }

  // GCed actor have not to be called GCed script `Destroy` function when Level is released.
  this->mActorMap.clear();
  this->mInitialized = false;
}

void FDyLevel::Update(_MIN_ float dt)
{
  if (this->mInitialized == false) { return; }
}

const DDyColorRGBA& FDyLevel::GetBackgroundColor() const noexcept
{
  return this->mLevelBackgroundColor;
}

const std::string& FDyLevel::GetLevelName() const noexcept
{
  return this->mLevelName;
}

void FDyLevel::SetBackgroundColor(_MIN_ const DDyColorRGBA& backgroundColor) noexcept
{
  this->mLevelBackgroundColor = backgroundColor;
}

void FDyLevel::MDY_PRIVATE_SPECIFIER(AlignActorsPosition)() noexcept
{
  // Update transform to reflect transform information.
  for (auto& [specifier, ptrsmtActor] : this->mActorMap)
  {
    if (MDY_CHECK_ISEMPTY(ptrsmtActor)) { continue; }
    MDY_NOTUSED const auto& _ = ptrsmtActor->GetTransform()->GetTransform();
  }
}

std::string FDyLevel::ToString()
{
MDY_NOT_IMPLEMENTED_ASSERT();
return MDY_INITIALIZE_EMPTYSTR;
}

} /// ::dy namespace