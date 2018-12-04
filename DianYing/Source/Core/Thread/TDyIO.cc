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
#include <Dy/Core/Thread/TDyIO.h>
#include <Dy/Management/IO/IODataManager.h>
#include <Dy/Management/IO/IOResourceManager.h>
#include <Dy/Management/IO/MetaInfoManager.h>

namespace dy
{

TDyIO::TDyIO()
{
  MDY_CALL_ASSERT_SUCCESS(dy::MDyMetaInfo::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyIOData::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyIOResource::Initialize());

  this->mMetaInfoManager    = &MDyMetaInfo::GetInstance();
  this->mIODataManager      = &MDyIOData::GetInstance();
  this->mIOResourceManager  = &MDyIOResource::GetInstance();
}

TDyIO::~TDyIO()
{
  MDY_CALL_ASSERT_SUCCESS(dy::MDyIOResource::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyIOData::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyMetaInfo::Release());

  this->mIOResourceManager  = nullptr;
  this->mIODataManager      = nullptr;
  this->mMetaInfoManager    = nullptr;
}

void TDyIO::operator()()
{
  /// @brief Waiting callback for condition variable for task queueing processing routine.
  static auto CbTaskQueueWaiting = [this]
  {
    return this->mIsThreadStopped == true || this->mIOTaskQueue.empty() == false;
  };

  /// @brief Assign picked to workers.
  static auto AssignTaskToWorkers = [](_MIN_ const DDyIOTask& task, _MIO_ TDyIO::TWorkerList& workerList)
  {
    for (auto& [instance, thread] : workerList)
    {
      const auto isSuccessful = instance->outTryAssign(task);
      if (isSuccessful == DY_SUCCESS) { return; }
    }

    MDY_UNEXPECTED_BRANCH();
  };

  MDY_CALL_ASSERT_SUCCESS(this->Initialize());
  while (true)
  {
    DDyIOTask task;
    {
      std::unique_lock<std::mutex> lock(this->mQueueMutex);
      this->mConditionVariable.wait(lock, CbTaskQueueWaiting);
      if (this->mIsThreadStopped == true && this->mIOTaskQueue.empty() == true) { break; }
      this->mWorkerSemaphore.Wait();

      task = this->mIOTaskQueue.top();
      this->mIOTaskQueue.pop();
    }

    AssignTaskToWorkers(task, this->mWorkerList);
  }

  this->Release();
}

EDySuccess TDyIO::Initialize()
{
  // Initialize IOWorkers
  for (auto& [instance, thread] : this->mWorkerList)
  {
    instance  = std::make_unique<TDyIOWorker>(MDyMetaInfo::GetInstance());
    thread    = std::thread(&TDyIOWorker::operator(), std::ref(*instance));
  }

  return DY_SUCCESS;
}

void TDyIO::Release()
{
  // Stop and release IOWorkers
  for (auto& [instance, thread] : this->mWorkerList)
  {
    instance->outTryStop();
    thread.join();
    instance = nullptr;
  }

  MDY_NOT_IMPLEMENTED_ASSERT();
}

void TDyIO::TryStop()
{
  {
    std::lock_guard<std::mutex> lock(this->mQueueMutex);
    this->mIsThreadStopped = true;
  }
  this->mConditionVariable.notify_one();
}

bool TDyIO::IsMetaInformationExist(_MIN_ const std::string& specifier, _MIN_ EDyResourceType type)
{
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(this->mMetaInfoManager), "MetaInformation manager must not be null.");
  switch (type)
  {
  case EDyResourceType::Script:     return this->mMetaInfoManager->IsScriptMetaInformationExist(specifier);
  case EDyResourceType::Model:      return this->mMetaInfoManager->IsModelMetaInfoExist(specifier);
  case EDyResourceType::GLShader:   return this->mMetaInfoManager->IsGLShaderMetaInfoExist(specifier);
  case EDyResourceType::Texture:    return this->mMetaInfoManager->IsTextureMetaInfoExist(specifier);
  case EDyResourceType::Material:   return this->mMetaInfoManager->IsMaterialMetaInfoExist(specifier);
  case EDyResourceType::WidgetMeta: return this->mMetaInfoManager->IsWidgetMetaInfoExist(specifier);
  default: MDY_UNEXPECTED_BRANCH(); break;
  }

  MDY_UNEXPECTED_BRANCH_BUT_RETURN(false);
}

bool TDyIO::IsReferenceInstanceExist(_MIN_ const std::string& specifier, _MIN_ EDyResourceType type, _MIN_ EDyResourceStyle style)
{
  switch (style)
  {
  case EDyResourceStyle::Information:
    return this->mRIInformationMap.IsReferenceInstanceExist(specifier, type);
  case EDyResourceStyle::Resource:
    return this->mRIResourceMap.IsReferenceInstanceExist(specifier, type);
  default: MDY_UNEXPECTED_BRANCH(); break;
  }

  MDY_UNEXPECTED_BRANCH_BUT_RETURN(false);
}

void TDyIO::TryEnlargeResourceScope(_MIN_ EDyScope scope, _MIN_ const std::string& specifier, _MIN_ EDyResourceType type, _MIN_ EDyResourceStyle style)
{
  // @TODO IMPELEMNT THIS
}

EDySuccess TDyIO::TryRetrieveReferenceInstanceFromGC(_MIN_ const std::string& specifier, _MIN_ EDyResourceType type, _MIN_ EDyResourceStyle style)
{
  // @TODO IMPELEMNT THIS
  return DY_SUCCESS;
}

EDySuccess TDyIO::CreateReferenceInstance(
    _MIN_ const std::string& specifier,
    _MIN_ EDyResourceType type,
    _MIN_ EDyResourceStyle style,
    _MIN_ EDyScope scope)
{
  switch (style)
  {
  case EDyResourceStyle::Information: return this->mRIInformationMap.CreateReferenceInstance(specifier, type, style, scope);
  case EDyResourceStyle::Resource:    return this->mRIResourceMap.CreateReferenceInstance(specifier, type, style, scope);
  default: MDY_UNEXPECTED_BRANCH();   break;
  }

  MDY_UNEXPECTED_BRANCH_BUT_RETURN(DY_FAILURE);
}

#define MDY_CALL_BUT_NOUSE_RESULT(__MAExpression__) \
  { MDY_NOTUSED const auto _ = __MAExpression__; }

EDySuccess TDyIO::TryEnqueueTask(
    _MIN_ const std::string& specifier,
    _MIN_ EDyResourceType resourceType, _MIN_ EDyResourceStyle resourceStyle,
    _MIN_ EDyScope scope)
{
  /// @brief Check RI is exist, so enlarge scope and update properties etc.
  static auto CheckAndUpdateReferenceInstance = [&, this]() -> bool
  {
    if (this->IsReferenceInstanceExist(specifier, resourceType, resourceStyle) == true)
    {
      this->TryEnlargeResourceScope(scope, specifier, resourceType, resourceStyle);
      return true;
    }
    if (this->mGarbageCollector.IsReferenceInstanceExist(specifier, resourceType, resourceStyle) == true)
    {
      MDY_CALL_ASSERT_SUCCESS(this->TryRetrieveReferenceInstanceFromGC(specifier, resourceType, resourceStyle));
      //this->mGarbageCollector.MoveInstanceFromGC(specifier, resourceType, resourceStyle);
      return true;
    }

    return false;
  };

  MDY_ASSERT(this->IsMetaInformationExist(specifier, resourceType) == true, "Meat information must be exist.");
  constexpr TU08 kDefaultPriority = 128;

  if (resourceStyle == EDyResourceStyle::Resource)
  {
    MDY_CALL_BUT_NOUSE_RESULT(this->TryEnqueueTask(specifier, resourceType, EDyResourceStyle::Information, scope));
  }
  if (const auto isExist = CheckAndUpdateReferenceInstance(); isExist == true) { return DY_SUCCESS; }

  MDY_CALL_ASSERT_SUCCESS(this->CreateReferenceInstance(specifier, resourceType, resourceStyle, scope));
  DDyIOTask task = {};
  { // Construct IO Task.
    task.mSpecifierName = specifier;
    task.mObjectStyle   = EDyObject::Etc_NotBindedYet;
    task.mPtrBoundObject= nullptr;
    task.mResourceType  = resourceType;
    task.mResourcecStyle= resourceStyle;
    task.mScope         = scope;
    task.mTaskPriority  = kDefaultPriority;
  }

  { // Insert task to queue
    std::unique_lock<std::mutex> lock(this->mQueueMutex);
    if (this->mIsThreadStopped == true) { return DY_FAILURE; }

    this->mIOTaskQueue.emplace(task);
  }

  this->mConditionVariable.notify_one();
  return DY_SUCCESS;
}

} /// :: dy namesapace