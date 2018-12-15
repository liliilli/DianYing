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
#include <Dy/Core/Thread/IO/DDyIOWorkerResult.h>
#include <Dy/Core/Resource/Type/TemplateRescInfoType.h>
#include <Dy/Core/Resource/Type/FDyBinderBase.h>

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
  /// @brief Create and populate resource which have specifier that can be found in meta information container. \n
  /// If resourceType is `Resource` but failed to find `Information` of populated container,
  /// fist populate `Information` and `Resource` later. \n
  /// If given specifier of resource type could not found in Meta container, causes UB or assertion error in debug mode.
  static void PopulateResource(_MIN_ const std::string& specifier, _MIN_ EDyResourceType resourceType, _MIN_ EDyResourceStyle resourceStyle, _MIN_ EDyScope scope);

  /// @brief Create and populate resource list which have specifier that can be found in meta info container. \n
  /// This function is list version of `PopulateResource` function above.
  static void PopulateResourceList(_MIN_ const std::vector<DDyResourceName>& specifierList, _MIN_ bool isWaited = false);

  /// @brief Try resource garbage collection manually. This function may causes time consuming, call this carefully.
  static void TryGC();

  /// @brief Try stop IO thread manually. this function is thread-safe to I/O Thread.
  static void TryStop();

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

  /// @brief Force try process IO Insert phase (letter of IO GC/IN phase)
  /// so let result instance insert into information & resource list, and let deferred task reinsert to task queue of I/O Thread.
  static void ForceProcessIOInsertPhase();

  /// @brief Check IO Thread is slept. True if only IO Worker are idle,
  /// Queue is empty and Result list also empty.
  static MDY_NODISCARD bool IsIOThreadSleep();

  /// @brief
  template <EDyResourceType TType>
  static MDY_NODISCARD std::optional<const __TResourceType_T<TType>*>
  TryRequireResource(_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder)
  {
    if constexpr (TType == EDyResourceType::GLShader)   { return pTryRequireResource_GLShader(iSpecifier, iPtrBinder); }
    else if constexpr (TType == EDyResourceType::Model) { return pTryRequireResource_Model(iSpecifier, iPtrBinder); }
    else
    {
      MDY_NOT_IMPLEMENTED_ASSERT();
      return std::nullopt;
    }
  }

private:
  static MDY_NODISCARD EDySuccess
  pTryRequireResource(_MIN_ const std::string& iSpecifier, _MIN_ EDyResourceType iType, _MIN_ const __FDyBinderBase* iPtrBinder);

  static MDY_NODISCARD std::optional<const __TResourceType_T<EDyResourceType::GLShader>*>
  pTryRequireResource_GLShader(_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder);

  static MDY_NODISCARD std::optional<const __TResourceType_T<EDyResourceType::Model>*>
  pTryRequireResource_Model(_MIN_ const std::string& iSpecifier, _MIN_ const __FDyBinderBase* iPtrBinder);
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_THREAD_SDYIOCONNECTIONHELPER_H