#ifndef GUARD_DY_CORE_THREAD_SDYIOCONNECTIONHELPER_H
#define GUARD_DY_CORE_THREAD_SDYIOCONNECTIONHELPER_H
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

#include <string>
#include <Dy/Core/Thread/IO/EDyIOTask.h>
#include <Dy/Core/Reflection/RDyBuiltinResources.h>
#include "IO/DDyIOWorkerResult.h"

//!
//! Forward declaration
//!

namespace dy
{
enum class EDyScope;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

///
/// @class SDyIOConnectionHelper
/// @brief IO Connection helper class for communicating to IOThread from Logic thread.
///
class SDyIOConnectionHelper final
{
public:
  ///
  /// @brief Create and populate resource which have specifier that can be found in meta information container. \n
  /// If resourceType is `Resource` but failed to find `Information` of populated container,
  /// fist populate `Information` and `Resource` later. \n
  /// If given specifier of resource type could not found in Meta container, causes UB or assertion error in debug mode.
  ///
  static void PopulateResource(_MIN_ const std::string& specifier, _MIN_ EDyResourceType resourceType, _MIN_ EDyResourceStyle resourceStyle, _MIN_ EDyScope scope);

  /// @brief Try resource garbage collection manually. This function may causes time consuming, call this carefully.
  static void TryGC();

  /// @brief Try stop IO thread manually. this function is thread-safe to I/O Thread.
  static void TryStop();

  ///
  /// @brief
  ///
  static void PopulateResources(_MIN_ const std::vector<DDyResourceName>& specifierList, _MIN_ bool isWaited = false);

  /// @brief Insert result instance from IO Worker.
  static void InsertResult(_MIN_ const DDyIOWorkerResult& result) noexcept;

  // @TODO MUST BE PRIVATE

  /// @brief Check IO Result in condition.
  static MDY_NODISCARD bool CheckIOResultInCondition() noexcept;

  /// @brief Check Main deferred task list is empty. \n
  static MDY_NODISCARD bool IsMainTaskListIsEmpty() noexcept;

  /// @brief Force Try process deferred task list which must be processed in main thread, \n
  /// so Insert created resource instance into result instance list for IO GC/IN Phase.
  static void ForceProcessDeferredMainTaskList();

#ifdef false
  /// @brief Try insert IO Result instance to each information & resource container.
  /// and let deferred queue insert queue with high priority if specifier and type is matched with result.
  static void ForceInsertIOResultToContainer();
#endif

  /// @brief Force try process IO Insert phase (letter of IO GC/IN phase)
  /// so let result instance insert into information & resource list, and let deferred task reinsert to task queue of I/O Thread.
  static void ForceProcessIOInsertPhase();

  /// @brief
  static MDY_NODISCARD bool IsIOThreadSleep();
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_THREAD_SDYIOCONNECTIONHELPER_H