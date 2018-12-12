#ifndef GAURD_DY_THREAD_IO_SDYIOWORKERCONNHELPER_H
#define GAURD_DY_THREAD_IO_SDYIOWORKERCONNHELPER_H
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

//!
//! Forward declaration
//!

namespace dy
{
struct DDyIOTask;
struct DDyIOWorkerResult;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

///
/// @class SDyIOWorkerConnHelper
/// @brief IO Connection helper class for communicating to IOThread from Logic thread.
///
class SDyIOWorkerConnHelper final
{
private:
  /// @brief Insert result instance from IO Worker.
  static void InsertResult(_MIN_ const DDyIOWorkerResult& result) noexcept;

  /// @brief Notify from IO.
  static void TryNotify();

  /// @brief Forward task to be processed main thread into `TDyIO::mIOProcessMainTaskList`.
  static void TryForwardToMainTaskList(_MIN_ const DDyIOTask& task) noexcept;

  friend class TDyIOWorker;
};

} /// ::dy namespace

#endif /// GAURD_DY_THREAD_IO_SDYIOWORKERCONNHELPER_H