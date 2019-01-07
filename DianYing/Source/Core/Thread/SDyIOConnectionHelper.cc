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
#include <Dy/Core/Thread/SDyIOConnectionHelper.h>
#include <Dy/Core/DyEngine.h>

namespace dy
{

void SDyIOConnectionHelper::PopulateResource(
    _MIN_ const std::string& specifier,
    _MIN_ EDyResourceType resourceType,
    _MIN_ EDyResourceStyle resourceStyle,
    _MIN_ EDyScope scope)
{
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(gEngine), "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();
  MDY_CALL_ASSERT_SUCCESS(ioThread.outTryEnqueueTask(specifier, resourceType, resourceStyle, scope));
}

void SDyIOConnectionHelper::TryStop()
{
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(gEngine), "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();
  ioThread.outTryStop();
}

void SDyIOConnectionHelper::TryGC(_MIN_ EDyScope iScope, _MIN_ EDyResourceStyle iStyle)
{
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(gEngine), "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();
  ioThread.outTryForwardCandidateRIToGCList(iScope, iStyle);
}

void SDyIOConnectionHelper::PopulateResourceList(
    _MIN_ const std::vector<DDyResourceName>& specifierList, 
    _MIN_ const EDyScope iScope,
    _MIN_ std::function<void(void)> callback)
{
  for (const auto& [type, specifier] : specifierList)
  {
    PopulateResource(specifier, type, EDyResourceStyle::Resource, iScope);
  }

  MDY_ASSERT(MDY_CHECK_ISNOTNULL(gEngine), "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();
  ioThread.BindSleepCallbackFunction(callback);
}

void SDyIOConnectionHelper::PopulateResourceList(
    _MIN_ const std::vector<std::vector<DDyResourceName>>& specifierList, 
    _MIN_ const EDyScope iScope,
    _MIN_ std::function<void(void)> callback)
{
  for (const auto& list : specifierList)
  {
    for (const auto& [type, specifier] : list)
    {
      PopulateResource(specifier, type, EDyResourceStyle::Resource, iScope);
    }
  }

  MDY_ASSERT(MDY_CHECK_ISNOTNULL(gEngine), "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();
  ioThread.BindSleepCallbackFunction(callback);
}

void SDyIOConnectionHelper::PopulateResourceList(
    _MIN_ const TDDyResourceNameSet& iSpecifierSet, 
    _MIN_ const EDyScope iScope,
    _MIN_ std::function<void()> iCallback)
{
  for (const auto& [type, specifier] : iSpecifierSet)
  {
    PopulateResource(specifier, type, EDyResourceStyle::Resource, iScope);
  }

  MDY_ASSERT(MDY_CHECK_ISNOTNULL(gEngine), "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();
  ioThread.BindSleepCallbackFunction(iCallback);
}

void SDyIOConnectionHelper::InsertResult(_MIN_ const DDyIOWorkerResult& result) noexcept
{
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(gEngine), "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();
  ioThread.outInsertResult(result);
}

bool SDyIOConnectionHelper::CheckIOResultInCondition() noexcept
{
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(gEngine), "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();
  return ioThread.outCheckIOResultInCondition();
}

bool SDyIOConnectionHelper::IsMainTaskListIsEmpty() noexcept
{
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(gEngine), "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();
  return ioThread.isoutIsMainTaskListIsEmpty();
}

void SDyIOConnectionHelper::ForceProcessDeferredMainTaskList()
{
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(gEngine), "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();
  ioThread.outMainForceProcessDeferredMainTaskList();
}

void SDyIOConnectionHelper::ForceProcessIOInsertPhase()
{
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(gEngine), "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();
  ioThread.outForceProcessIOInsertPhase();
}

bool SDyIOConnectionHelper::IsIOThreadSleep()
{
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(gEngine), "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();
  return ioThread.outIsIOThreadSlept();
}

EDySuccess SDyIOConnectionHelper::TryCallSleptCallbackFunction()
{
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(gEngine), "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();
  return ioThread.outTryCallSleptCallbackFunction();
}

} /// ::dy namespace