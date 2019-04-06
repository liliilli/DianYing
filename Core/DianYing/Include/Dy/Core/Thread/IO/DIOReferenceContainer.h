#ifndef GUARD_DY_CORE_THREAD_IO_DDYREFERENCECONTAINER_H
#define GUARD_DY_CORE_THREAD_IO_DDYREFERENCECONTAINER_H
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

#include <Dy/Core/Thread/IO/DIOReferenceInstance.h>

//!
//! Forward declaration
//!

namespace dy
{
MDY_INTERFACE IBinderBase;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

/// @class DIOReferenceContainer
/// @brief IO Reference container for managing reference counting.
class DIOReferenceContainer final
{
public:
  using TRefInstanceMap = TStringHashMap<std::unique_ptr<DIOReferenceInstance>>;

  /// @brief Check whether resource's Reference Instance is exist on now as any kind of information.
  /// @param specifier Resource specifier name.
  /// @param type  Resource type.
  MDY_NODISCARD bool IsReferenceInstanceExist(_MIN_ const std::string& specifier, _MIN_ EResourceType type) const noexcept;

  /// @brief Check whether resource's Reference Instance is exist and bound by any heap resource.
  /// If not exist instance, UB might be occurred.
  MDY_NODISCARD bool IsReferenceInstanceBound(_MIN_ const std::string& specifier, _MIN_ EResourceType type) const noexcept;

  /// @brief Try update scope of given style's specifier RI of resource type. \n
  /// This function is not checking RI(Reference Instance) is exist or not, so when RI is not exist
  /// may will cause UB (Undefined Behaviour).
  /// \n\n
  /// If found RI's scope is \n
  /// Global < Level \n
  /// Level  < Temporal \n
  /// RI's scope will be changed to big range.
  bool TryEnlargeResourceScope(_MIN_ EResourceScope scope, _MIN_ const std::string& specifier, _MIN_ EResourceType type);

  /// @brief Try to bind binder instance to valid resource Reference Instance.
  /// If not exist, just do nothing and return DY_FAILURE.
  MDY_NODISCARD EDySuccess TryBindBinderToResourceRI(
    const std::string& iSpecifier, 
    EResourceType iType, 
    IBinderBase& iPtrBinder);

  /// @brief Try detach binder instance from valid resource Reference Instance.
  /// If not exist, just do nothing and return DY_FAILURE.
  MDY_NODISCARD EDySuccess TryDetachBinderFromResourceRI(
    const std::string& iSpecifier, 
    EResourceType iType, 
    IBinderBase& iPtrBinder);

  /// @brief Get GC-Candidate Reference instance list from container. \n
  /// Condition-satisfied RI will be removed from container.
  std::vector<std::unique_ptr<DIOReferenceInstance>>
  GetForwardCandidateRIAsList(EResourceScope iScope);

  /// @brief Create new reference instance. `specifier` must not be duplicated given type, style container.
  MDY_NODISCARD EDySuccess CreateReferenceInstance(
      _MIN_ const std::string& specifier,
      _MIN_ EResourceType type, _MIN_ EResourceStyle style, _MIN_ EResourceScope scope);
  
  /// @brief Move old Reference Instance.
  MDY_NODISCARD EDySuccess MoveReferenceInstance(std::unique_ptr<DIOReferenceInstance>&& ioRi);

  /// @brief Try update validity of this reference instance. \n
  /// If `isValid` is true, `iPtrInstance` must point to something valid, should not be null.
  MDY_NODISCARD EDySuccess TryUpdateValidity(
    EResourceType type, 
    const std::string& specifier, 
    bool isValid,
    void* iPtrInstance = nullptr);

private:
  /// @brief Forward GCed candidate Reference Instance list to `iResult`.
  /// Gced candidate must have 0 item of __BinderBase.
  void ForwardCandidateRIFromList(
      EResourceScope iScope,
      TStringHashMap<std::unique_ptr<DIOReferenceInstance>>& ioContainer, 
      std::vector<std::unique_ptr<DIOReferenceInstance>>& oResult);

  TRefInstanceMap mMapTextureReference     = {};
  TRefInstanceMap mMapGLShaderReference    = {};
  TRefInstanceMap mMapMeshReference        = {};
  TRefInstanceMap mMapModelReference       = {};
  TRefInstanceMap mMapMaterialReference    = {};
  TRefInstanceMap mMapAttachmentReference  = {};
  TRefInstanceMap mMapFrameBufferReference = {};
  TRefInstanceMap mMapModelSkeletonReference   = {};
  TRefInstanceMap mMapModelAnimScrapReference  = {};
  TRefInstanceMap mMapSoundReference           = {};
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_THREAD_IO_DDYREFERENCECONTAINER_H