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

void SDyIOConnectionHelper::TryGC()
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void SDyIOConnectionHelper::TryStop()
{
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(gEngine), "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();
  ioThread.outTryStop();
}

void SDyIOConnectionHelper::PopulateResources(_MIN_ const std::vector<DDyResourceName>& specifierList, _MIN_ bool isWaited)
{
  MDY_ASSERT(isWaited == true, "non-wait version not supported.");
  for (const auto& [type, specifier] : specifierList)
  {
    PopulateResource(specifier, type, EDyResourceStyle::Resource, EDyScope::Global);
  }
}

bool SDyIOConnectionHelper::CheckIOResultInCondition() noexcept
{
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(gEngine), "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();
  return ioThread.outCheckIOResultInCondition();
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

} /// ::dy namespace