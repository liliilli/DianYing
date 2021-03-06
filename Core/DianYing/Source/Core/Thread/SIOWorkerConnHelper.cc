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
#include <Dy/Core/Thread/SIOWorkerConnHelper.h>
#include <Dy/Core/GEngine.h>

namespace dy
{

void SIOWorkerConnHelper::InsertResult(_MIN_ const DRescIOWorkerResult& result) noexcept
{
  MDY_ASSERT_MSG(TEMP_CAST(gEngine) != nullptr, "TEMP_CAST(gEngine) must not be null.");
  auto& ioThread = *TEMP_CAST(gEngine)->pfGetIOThread();
  ioThread.SyncInsertResult(result);
}

void SIOWorkerConnHelper::TryForwardToMainTaskList(_MIN_ const DRescIOTask& task) noexcept
{
  MDY_ASSERT_MSG(TEMP_CAST(gEngine) != nullptr, "TEMP_CAST(gEngine) must not be null.");
  auto& ioThread = *TEMP_CAST(gEngine)->pfGetIOThread();
  ioThread.SyncTryForwardTaskToMainList(task);
}

} /// ::dy namespace