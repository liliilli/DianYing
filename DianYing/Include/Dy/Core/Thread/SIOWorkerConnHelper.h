#ifndef GAURD_DY_THREAD_IO_SDYIOWORKERCONNHELPER_H
#define GAURD_DY_THREAD_IO_SDYIOWORKERCONNHELPER_H
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

//!
//! Forward declaration
//!

namespace dy
{
struct DRescIOTask;
struct DRescIOWorkerResult;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

///
/// @class SIOWorkerConnHelper
/// @brief IO Connection helper class for communicating to IOThread from IO worker thread.
///
class SIOWorkerConnHelper final
{
private:
  /// @brief Insert result instance from IO Worker.
  static void InsertResult(_MIN_ const DRescIOWorkerResult& result) noexcept;

  /// @brief Forward task to be processed main thread into `TRescIO::mIOProcessMainTaskList`.
  static void TryForwardToMainTaskList(_MIN_ const DRescIOTask& task) noexcept;

  friend class TRescIOWorker;
};

} /// ::dy namespace

#endif /// GAURD_DY_THREAD_IO_SDYIOWORKERCONNHELPER_H