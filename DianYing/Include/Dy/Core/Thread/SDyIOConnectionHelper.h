#ifndef GUARD_DY_CORE_THREAD_SDYIOCONNECTIONHELPER_H
#define GUARD_DY_CORE_THREAD_SDYIOCONNECTIONHELPER_H
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

#include <string>
#include <Dy/Core/Thread/IO/EDyIOTask.h>
#include <Dy/Core/Thread/IO/DDyIOWorkerResult.h>

//!
//! Forward declaration
//!

namespace dy
{
struct DDyIOReferenceInstance;
struct  PDyMaterialInstanceMetaInfo;
enum class EResourceScope;
template <EResourceType TType> class TResourceBinder;
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
  static void PopulateResource(
      _MIN_ const std::string& specifier, 
      _MIN_ EResourceType resourceType, 
      _MIN_ EDyResourceStyle resourceStyle, 
      _MIN_ EResourceScope scope);

  /// @brief Create and populate resource list which have specifier that can be found in meta info container. \n
  /// This function is list version of `PopulateResource` function above.
  static void PopulateResourceList(
      _MIN_ const std::vector<DDyResourceName>& specifierList, 
      _MIN_ const EResourceScope iScope,
      _MIN_ std::function<void(void)> callback);

  /// @brief Overload version of `PopulateResourceList` above.
  static void PopulateResourceList(
      _MIN_ const std::vector<std::vector<DDyResourceName>>& specifierList, 
      _MIN_ const EResourceScope iScope,
      _MIN_ std::function<void(void)> callback);
  
  /// @brief Overload version of `PopulateResourceList` above.
  static void PopulateResourceList(
      _MIN_ const TDDyResourceNameSet& iSpecifierSet, 
      _MIN_ const EResourceScope iScope,
      _MIN_ std::function<void(void)> iCallback);

  /// @brief Create and populate instant material resource, that is not dependent on any material information.
  /// but dependent on given texture and shader. \n
  /// When callback is setup, instant material scope have callback condition, not temporary.
  static void PopulateInstantMaterialResource(
      _MIN_ PDyMaterialInstanceMetaInfo& desc, 
      _MIN_ TResourceBinder<EResourceType::Material>& refMat, 
      _MIN_ bool(*callback)() = nullptr);

  /// @brief Check Reference Instance (RI) which has a given type, style and specifier name is exist.
  static bool IsReferenceInstanceExist(
      _MIN_ const std::string& iSpecifier,
      _MIN_ EResourceType iType,
      _MIN_ EDyResourceStyle iStyle);

  /// @brief Try Garbage collect of Reference Instance with resource as Scope and Style, which
  /// is only Valid resource but count is 0. \n
  /// This function may causes time consuming, call this carefully.
  static void TryGC(_MIN_ EResourceScope iScope, _MIN_ EDyResourceStyle iStyle);

  /// @brief Try stop IO thread manually. this function is thread-safe to I/O Thread.
  static void TryStop();

  /// @brief Insert result instance from IO Worker.
  static void InsertResult(_MIN_ const DDyIOWorkerResult& result) noexcept;

  /// @brief Insert RI to gcList anyway. Use this carefully.
  static void InsertGcCandidate(std::unique_ptr<DDyIOReferenceInstance>&& iRefRI);

  // @TODO MUST BE PRIVATE

  /// @brief Check IO Result in condition.
  MDY_NODISCARD static bool CheckIOResultInCondition() noexcept;

  /// @brief Check Main deferred task list is empty. \n
  MDY_NODISCARD static bool IsMainTaskListIsEmpty() noexcept;

  /// @brief Force Try process deferred task list which must be processed in main thread, \n
  /// so Insert created resource instance into result instance list for IO GC/IN Phase.
  static void ForceProcessDeferredMainTaskList();

  /// @brief Force try process IO Insert phase (letter of IO GC/IN phase)
  /// so let result instance insert into information & resource list, and let deferred task reinsert to task queue of I/O Thread.
  static void ForceProcessIOInsertPhase();

  /// @brief Check IO Thread is slept. True if only IO Worker are idle,
  /// Queue is empty and Result list also empty.
  MDY_NODISCARD static bool IsIOThreadSleep();

  /// @brief Try call callback function when if only Thread I/O is slept.
  /// If not slept or exist callback function, just return DY_FAILURE.
  static EDySuccess TryCallSleptCallbackFunction();
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_THREAD_SDYIOCONNECTIONHELPER_H