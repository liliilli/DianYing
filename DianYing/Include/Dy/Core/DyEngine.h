#ifndef GUARD_DY_CORE_MDYENGINE_H
#define GUARD_DY_CORE_MDYENGINE_H
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

#include <Dy/Management/Interface/ISingletonCrtp.h>
#include <Dy/Core/Thread/TDyIO.h>
#include <Dy/Helper/Pointer.h>

//!
//! Forward declaration
//!

namespace dy
{
class MDyTime;
class MDyWindow;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

///
/// @class DyEngine
/// @brief
///
class DyEngine final : public IDySingleton<DyEngine>
{
  MDY_SINGLETON_PROPERTIES(DyEngine);
  MDY_SINGLETON_DERIVED(DyEngine);
public:
  ///
  /// @brief Run engine.
  ///
  void operator()();

  /// @brief Get time manager reference.
  MDY_NODISCARD MDyTime&    GetTimeManager();
  /// @brief Get window manager reference.
  MDY_NODISCARD MDyWindow&  GetWindowManager();

private:
  /// DO NOT TOUCH THIS!
  TDyIO*        mIOThreadInstance = nullptr;
  std::thread   mIOThread;
};

inline DyEngine* gEngine = nullptr;

} /// ::dy namespace

#endif /// GUARD_DY_CORE_MDYENGINE_H