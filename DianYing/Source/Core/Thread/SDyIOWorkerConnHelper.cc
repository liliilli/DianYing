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
#include <Dy/Core/Thread/SDyIOWorkerConnHelper.h>
#include <Dy/Core/GDyEngine.h>

namespace dy
{

void SDyIOWorkerConnHelper::InsertResult(_MIN_ const DDyIOWorkerResult& result) noexcept
{
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(gEngine), "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();
  ioThread.outInsertResult(result);
}

void SDyIOWorkerConnHelper::TryNotify()
{
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(gEngine), "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();
  ioThread.outTryNotifyWorkerIsIdle();
}

void SDyIOWorkerConnHelper::TryForwardToMainTaskList(_MIN_ const DDyIOTask& task) noexcept
{
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(gEngine), "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();
  ioThread.outTryForwardToMainTaskList(task);
}

} /// ::dy namespace