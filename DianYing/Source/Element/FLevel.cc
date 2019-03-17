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
#include <Dy/Element/FLevel.h>
#include <Dy/Management/MWorld.h>
#include <Dy/Element/Type/PActorCreationDescriptor.h>
#include <Dy/Meta/Information/ElementLevelMetaInfo.h>
#include <Dy/Component/CTransform.h>

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

FLevel::FLevel(_MIN_ const PDyLevelConstructMetaInfo& desc)
{
  this->mLevelName            = desc.mMeta.mLevelName;
  this->mLevelBackgroundColor = desc.mMeta.mLevelBackgroundColor;

  for (const auto& objectInformation : desc.mLevelObjectMetaInfoList)
  { // Create object, FActor
    switch (objectInformation->mObjectType)
    {
    case EWorldObjectType::Actor:
    { // General object type. Make FActor instance.
      auto instancePtr = std::make_unique<FActor>(*objectInformation);
      auto [it, result] = this->mActorMap.try_emplace(instancePtr->GetActorName(), std::move(instancePtr));
      MDY_ASSERT_MSG(result == true, "Unexpected error occured in inserting FActor to object map.");
    } break;
    case EWorldObjectType::SceneScriptor:  MDY_NOT_IMPLEMENTED_ASSERT(); break;
    case EWorldObjectType::Object:         MDY_NOT_IMPLEMENTED_ASSERT(); break;
    default: MDY_UNEXPECTED_BRANCH(); break;
    }
  }

  DyPushLogInfo("{} | Initialize level context. | Level name : {}", "FLevel::Initialize()", this->mLevelName);
  this->mInitialized = true;
}

FLevel::~FLevel()
{
  DyPushLogInfo("{} | Release level context. | Level name : {}", "FLevel::Release()", this->mLevelName);
  for (auto& [name, actor] : this->mActorMap)
  {
    if (actor == nullptr) { continue; }
    MWorld::GetInstance().pfMoveActorToGc(DyMakeNotNull(actor.release()));
  }

  // GCed actor have not to be called GCed script `Destroy` function when Level is released.
  this->mActorMap.clear();
  this->mInitialized = false;
}

FActor* FLevel::GetActorWithObjectId(_MIN_ TU32 iObjectId) noexcept
{
  for (auto& [actorName, smtptrObject] : this->mActorMap)
  {
    if (smtptrObject == nullptr) { continue; }
    // Check id.
    if (smtptrObject->GetId() == iObjectId) { return smtptrObject.get(); }
    // Iterate id.
    auto* ptrReturnedObject = smtptrObject->GetActorWithObjectId(iObjectId);
    if (ptrReturnedObject != nullptr) { return ptrReturnedObject; }
  }
  return nullptr;
}

std::vector<NotNull<FActor*>> 
FLevel::GetAllActorsWithTag(_MIN_ const std::string& iTagSpecifier) const noexcept
{
  std::vector<NotNull<FActor*>> result;
  for (const auto& [specifier, ptrsmtActor] : this->mActorMap)
  {
    if (MDY_CHECK_ISEMPTY(ptrsmtActor)) { continue; }
    if (ptrsmtActor->GetActorTag() == iTagSpecifier) { result.emplace_back(ptrsmtActor.get()); }
  }

  return result;
}

std::vector<NotNull<FActor*>> 
FLevel::GetAllActorsWithTagRecursive(_MIN_ const std::string& iTagSpecifier) const noexcept
{
  std::vector<NotNull<FActor*>> result;
  for (const auto& [specifier, ptrsmtActor] : this->mActorMap)
  {
    if (MDY_CHECK_ISEMPTY(ptrsmtActor)) { continue; }
    if (ptrsmtActor->GetActorTag() == iTagSpecifier) { result.emplace_back(ptrsmtActor.get()); }
    if (ptrsmtActor->HasChildrenActor() == true)
    {
      const auto subResult = ptrsmtActor->GetAllActorsWithTagRecursive(iTagSpecifier);
      result.insert(result.end(), MDY_BIND_BEGIN_END(subResult));
    }
  }

  return result;
}

std::vector<NotNull<FActor*>> 
FLevel::GetAllActorsWithName(_MIN_ const std::string& iNameSpecifier) const noexcept
{
  if (iNameSpecifier.empty() == true)   { return {}; }

  std::vector<NotNull<FActor*>> result;
  for (const auto& [specifier, ptrsmtActor] : this->mActorMap)
  {
    if (MDY_CHECK_ISEMPTY(ptrsmtActor)) { continue; }
    if (ptrsmtActor->GetActorName() == iNameSpecifier) { result.emplace_back(ptrsmtActor.get()); }
  }

  return result;;
}

std::vector<NotNull<FActor*>> 
FLevel::GetAllActorsWithNameRecursive(_MIN_ const std::string& iNameSpecifier) const noexcept
{
  if (iNameSpecifier.empty() == true)   { return {}; }

  std::vector<NotNull<FActor*>> result;
  for (const auto& [specifier, ptrsmtActor] : this->mActorMap)
  {
    if (MDY_CHECK_ISEMPTY(ptrsmtActor)) { continue; }
    if (ptrsmtActor->GetActorName() == iNameSpecifier) { result.emplace_back(ptrsmtActor.get()); }
    if (ptrsmtActor->HasChildrenActor() == true)
    {
      const auto subResult = ptrsmtActor->GetAllActorsWithNameRecursive(iNameSpecifier);
      result.insert(result.end(), MDY_BIND_BEGIN_END(subResult));
    }
  }

  return result;;
}

FActor* FLevel::GetActorWithFullName(_MIN_ const std::string& iFullName) const noexcept
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  return nullptr;
}

FActor::TActorMap& FLevel::GetActorContainer() noexcept
{
  return this->mActorMap;
}

void FLevel::CreateActorInstantly(_MIN_ const PActorCreationDescriptor& descriptor)
{
  // If parent full specifier name is empty so descripor requires make actor on root.
  if (descriptor.mParentFullSpecifierName.empty() == true)
  {
    // Get Auto generated name.
    PActorCreationDescriptor desc = descriptor;
    desc.mActorSpecifierName = this->TryGetGeneratedName(descriptor.mActorSpecifierName);

    auto instancePtr  = std::make_unique<FActor>(desc);
    auto [it, result] = this->mActorMap.try_emplace(instancePtr->GetActorName(), std::move(instancePtr));
    MDY_ASSERT_MSG(result == true, "Unexpected error occured in inserting FActor to object map.");
     
    // Try propagate transform.
    auto& [specifier, ptrsmtActor] = *it;
    if (ptrsmtActor->HasParent() == true)
    {
      ptrsmtActor->GetPtrParent()->GetTransform()->TryPropagateTransformToChildren();
    }
  }
  else
  { // Or otherwise...
    auto* ptrParent = this->GetActorWithFullName(descriptor.mParentFullSpecifierName);
    // If parent is not exist because removed or will be removed on this frame, do nothing and do not create.
    if (MDY_CHECK_ISNULL(ptrParent)) { return; }

    MDY_NOT_IMPLEMENTED_ASSERT();
  }
}

void FLevel::Update(_MIN_ float dt)
{
  if (this->mInitialized == false) { return; }
}

const DColorRGBA& FLevel::GetBackgroundColor() const noexcept
{
  return this->mLevelBackgroundColor;
}

const std::string& FLevel::GetLevelName() const noexcept
{
  return this->mLevelName;
}

void FLevel::SetBackgroundColor(_MIN_ const DColorRGBA& backgroundColor) noexcept
{
  this->mLevelBackgroundColor = backgroundColor;
}

void FLevel::MDY_PRIVATE(AlignActorsPosition)() noexcept
{
  // Update transform to reflect transform information.
  for (auto& [specifier, ptrsmtActor] : this->mActorMap)
  {
    if (MDY_CHECK_ISEMPTY(ptrsmtActor)) { continue; }
    ptrsmtActor->GetTransform()->TryPropagateTransformToChildren();
  }
}

std::string FLevel::ToString()
{
MDY_NOT_IMPLEMENTED_ASSERT();
return MDY_INITIALIZE_EMPTYSTR;
}

} /// ::dy namespace