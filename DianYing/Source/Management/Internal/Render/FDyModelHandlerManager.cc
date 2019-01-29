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
#include <Dy/Management/Internal/Render/FDyModelHandlerManager.h>
#include <Dy/Core/Thread/SDyIOConnectionHelper.h>
#include <Dy/Management/IO/MetaInfoManager.h>
#include <Dy/Management/LoggingManager.h>

namespace dy
{

EDySuccess FDyModelHandlerManager::pfInitialize() 
{
  return DY_SUCCESS;
}

EDySuccess FDyModelHandlerManager::pfRelease()
{
  return DY_SUCCESS;
}

bool FDyModelHandlerManager::IsBoundModelExist(_MIN_ const std::string& iModelSpecifier) const noexcept
{
  return DyIsMapContains(this->mModelHandlerContainer, iModelSpecifier);
}

EDySuccess FDyModelHandlerManager::TryCreateHandler(_MIN_ const std::string& iModelSpecifier) noexcept
{
  if (SDyIOConnectionHelper::IsReferenceInstanceExist(
      iModelSpecifier, 
      EDyResourceType::Model, 
      EDyResourceStyle::Resource) == false)
  {
    // Check if model is exist in meta information manager (so, Model can be required)
    const auto& metaManager = MDyMetaInfo::GetInstance();
    if (metaManager.IsModelMetaInfoExist(iModelSpecifier) == false) { return DY_FAILURE; }

    // If model (not instantiated) is exist on container, just create and require model resource.
    auto ptrsmtHandler = std::make_unique<DDyModelHandler>(iModelSpecifier);
    auto [it, isSucceeded] = this->mModelHandlerContainer.try_emplace(iModelSpecifier, std::move(ptrsmtHandler));
    MDY_ASSERT_FORCE(isSucceeded == true, "Unexpected error occurred.");

    return DY_SUCCESS;
  }
  else
  {
    // If model (not instantiated) is exist, just create and require model resource.
    auto ptrsmtHandler = std::make_unique<DDyModelHandler>(iModelSpecifier);
    auto [it, isSucceeded] = this->mModelHandlerContainer.try_emplace(iModelSpecifier, std::move(ptrsmtHandler));
    MDY_ASSERT_FORCE(isSucceeded == true, "Unexpected error occurred.");
    
    return DY_SUCCESS;
  }
}

EDySuccess FDyModelHandlerManager::BindToHandler(_MIN_ const std::string& iSpecifier, _MIN_ FDyActor& iRefActor, _MIN_ CDyModelFilter& iFilter)
{
  // Check
  if (this->IsBoundModelExist(iSpecifier) == false) { return DY_FAILURE; }

  // And check.
  auto& ptrsmtHandler = this->mModelHandlerContainer[iSpecifier];
  if (ptrsmtHandler->IsActorItemExist(iRefActor) == false)
  {
    ptrsmtHandler->TryCreateActorBinding(iRefActor);
  }

  // Bind filter.
  ptrsmtHandler->BindFilter(iRefActor, iFilter);
  return DY_SUCCESS;
}

EDySuccess FDyModelHandlerManager::UnbindToHandler(_MIN_ const std::string& iSpecifier, _MIN_ FDyActor& iRefActor)
{
  // Check
  if (this->IsBoundModelExist(iSpecifier) == false) { return DY_FAILURE; }

  // And check.
  auto& ptrsmtHandler = this->mModelHandlerContainer[iSpecifier];
  if (ptrsmtHandler->IsActorItemExist(iRefActor) == false) { return DY_FAILURE; }

  // Unbind filter.
  ptrsmtHandler->UnbindFilter(iRefActor);
  return DY_SUCCESS;
}

bool FDyModelHandlerManager::IsActorInfoNeedToBeGc(_MIN_ const std::string& iSpecifier, _MIN_ FDyActor& iRefActor) const noexcept
{
  // Check
  if (this->IsBoundModelExist(iSpecifier) == false) 
  { 
    MDY_LOG_ERROR_D("Could not find bound model instance. {}", iSpecifier);
    return DY_FAILURE; 
  }

  // And check..
  const auto& ptrsmtHandler = this->mModelHandlerContainer.at(iSpecifier);
  if (ptrsmtHandler->IsActorItemExist(iRefActor) == false) 
  { 
    MDY_LOG_ERROR_D("Could not find actor in model instance, {}.", iSpecifier);
    return DY_FAILURE; 
  }

  return ptrsmtHandler->IsActorNeedToBeGc(iRefActor);
}

EDySuccess FDyModelHandlerManager::TryRemoveBoundActor(_MIN_ const std::string& iSpecifier, _MIN_ FDyActor& iRefActor)
{
  // Check
  if (this->IsBoundModelExist(iSpecifier) == false)
  { 
    MDY_LOG_ERROR_D("Could not find bound model instance, {}.", iSpecifier);
    return DY_FAILURE; 
  }

  auto& ptrsmtHandler = this->mModelHandlerContainer[iSpecifier];
  return ptrsmtHandler->TryRemoveActorBinding(iRefActor);
}

bool FDyModelHandlerManager::IsBoundModelNeedToGc(_MIN_ const std::string& iSpecifier) const noexcept
{
  // If not found, just return FAILURE.
  if (this->IsBoundModelExist(iSpecifier) == false) { return false; }

  // Check emptiness.
  const auto& ptrsmtHandler = this->mModelHandlerContainer.at(iSpecifier);
  return ptrsmtHandler->IsEmpty();
}

EDySuccess FDyModelHandlerManager::TryRemoveBoundModel(_MIN_ const std::string& iSpecifier)
{
  // Check 
  if (this->IsBoundModelExist(iSpecifier) == false
  ||  this->IsBoundModelNeedToGc(iSpecifier) == false) 
  { 

    return DY_FAILURE; 
  }
  
  this->mModelHandlerContainer.erase(iSpecifier);
  return DY_SUCCESS;
}

} /// ::dy namespace