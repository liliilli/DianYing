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

void SDyIOWorkerConnHelper::InsertResult(_MIN_ const DRescIOWorkerResult& result) noexcept
{
  MDY_ASSERT_MSG(gEngine != nullptr, "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();
  ioThread.SyncInsertResult(result);
}

void SDyIOWorkerConnHelper::TryForwardToMainTaskList(_MIN_ const DRescIOTask& task) noexcept
{
  MDY_ASSERT_MSG(gEngine != nullptr, "gEngine must not be null.");
  auto& ioThread = *gEngine->pfGetIOThread();
  ioThread.SyncTryForwardTaskToMainList(task);
}

} /// ::dy namespace