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
#include <Dy/Management/Type/Render/DDyModelHandler.h>
#include <Dy/Helper/Library/HelperContainer.h>

namespace dy
{

DDyModelHandler::DDyModelHandler(_MIN_ const std::string& iModelSpecifier)
{
  // Try require resource.
  this->mModelBinder.TryRequireResource(iModelSpecifier);
}

bool DDyModelHandler::IsActorItemExist(_MIN_ FActor& iRefActor) const noexcept
{
  return Contains(this->mActorInformation, &iRefActor);
}

bool DDyModelHandler::IsActorNeedToBeGc(_MIN_ FActor& iRefActor) const noexcept
{
  const auto& item = this->mActorInformation.at(&iRefActor);

  return item.mPtrCompModelFilter == nullptr
      && item.mPtrModelRenderer == nullptr;
}

bool DDyModelHandler::IsEmpty() const noexcept
{
  return this->mActorInformation.empty();
}

bool DDyModelHandler::IsModelResourceValid() const noexcept
{
  return this->mModelBinder.IsResourceExist();
}

EDySuccess DDyModelHandler::TryCreateActorBinding(_MIN_ FActor& iRefActor)
{
  // If already exist, just return EDySuccess::DY_FAILURE.
  if (this->IsActorItemExist(iRefActor) == true) { return EDySuccess::DY_FAILURE; }

  // Otherwise, just return EDySuccess::DY_SUCCESS with creation.
  auto [it, isSucceeded] = this->mActorInformation.try_emplace(&iRefActor, DActorInfo{});
  MDY_ASSERT_MSG_FORCE(isSucceeded == true, "Unexpected error occurred.");

  return EDySuccess::DY_SUCCESS;
}

EDySuccess DDyModelHandler::TryRemoveActorBinding(_MIN_ FActor& iRefActor)
{
  if (this->IsActorNeedToBeGc(iRefActor) == false) 
  { 
    
    return EDySuccess::DY_FAILURE; 
  }

  this->mActorInformation.erase(&iRefActor);
  return EDySuccess::DY_SUCCESS;
}

EDySuccess DDyModelHandler::BindFilter(_MIN_ FActor& iRefActor, _MIN_ CModelFilter& iRefFilter)
{
  // If not exist, return EDySuccess::DY_FAILURE.
  if (this->IsActorItemExist(iRefActor) == false) { return EDySuccess::DY_FAILURE; }

  // Otherwise, bind filter.
  auto& item = this->mActorInformation[&iRefActor];
  item.mPtrCompModelFilter = &iRefFilter;
  return EDySuccess::DY_SUCCESS;
}

EDySuccess DDyModelHandler::BindRenderer(_MIN_ FActor& iRefActor, _MIN_ CModelRenderer& iRefComp)
{
  // If not exist, return EDySuccess::DY_FAILURE.
  if (this->IsActorItemExist(iRefActor) == false) { return EDySuccess::DY_FAILURE; }

  // Otherwise, bind filter.
  auto& item = this->mActorInformation[&iRefActor];
  item.mPtrModelRenderer = &iRefComp;
  return EDySuccess::DY_SUCCESS;
}

EDySuccess DDyModelHandler::BindAnimator(_MIN_ FActor& iRefActor, _MIN_ CModelAnimator& iRefComp)
{
  // If not exist, return EDySuccess::DY_FAILURE.
  if (this->IsActorItemExist(iRefActor) == false) { return EDySuccess::DY_FAILURE; }

  // Otherwise, bind filter.
  auto& item = this->mActorInformation[&iRefActor];
  item.mPtrModelAnimator = &iRefComp;
  return EDySuccess::DY_SUCCESS;
}

EDySuccess DDyModelHandler::UnbindFilter(_MIN_ FActor& iRefActor)
{
  // If not exist, return EDySuccess::DY_FAILURE.
  if (this->IsActorItemExist(iRefActor) == false) { return EDySuccess::DY_FAILURE; }

  auto& item = this->mActorInformation[&iRefActor];
  item.mPtrCompModelFilter = nullptr;
  return EDySuccess::DY_SUCCESS;
}

EDySuccess DDyModelHandler::UnbindRenderer(_MIN_ FActor& iRefActor)
{
  // If not exist, return EDySuccess::DY_FAILURE.
  if (this->IsActorItemExist(iRefActor) == false) { return EDySuccess::DY_FAILURE; }

  auto& item = this->mActorInformation[&iRefActor];
  item.mPtrModelRenderer = nullptr;
  return EDySuccess::DY_SUCCESS;
}

EDySuccess DDyModelHandler::UnbindAnimator(_MIN_ FActor& iRefActor)
{
  // If not exist, return EDySuccess::DY_FAILURE.
  if (this->IsActorItemExist(iRefActor) == false) { return EDySuccess::DY_FAILURE; }

  auto& item = this->mActorInformation[&iRefActor];
  item.mPtrModelAnimator = nullptr;
  return EDySuccess::DY_SUCCESS;
}

const DDyModelHandler::TContainer& DDyModelHandler::GetActorContainer() const noexcept
{
  return this->mActorInformation;
}

DDyModelHandler::TContainer& DDyModelHandler::GetActorContainer() noexcept
{
  return this->mActorInformation;
}

const TDyResourceBinderModel& DDyModelHandler::GetModelBinderReference() const noexcept
{
  return this->mModelBinder;
}

} /// ::dy namespace