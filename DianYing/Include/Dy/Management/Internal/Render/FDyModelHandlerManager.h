#ifndef GUARD_DY_MANAGEMENT_INTENRAL_RENDER_FDyModelHandlerManager_H
#define GUARD_DY_MANAGEMENT_INTENRAL_RENDER_FDyModelHandlerManager_H
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

#include <Dy/Management/Interface/ISingletonCrtp.h>
#include <Dy/Management/Type/Render/DDyModelHandler.h>

//!
//! Forward declaration
//!

namespace dy
{
class CModelAnimator;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

/// @class FDyModelHandlerManager
/// @brief Internal model handler manager. \n 
/// This manager initialized and released automatically in MRendering.
class FDyModelHandlerManager final : public ISingleton<FDyModelHandlerManager>
{
public:
  MDY_SINGLETON_DERIVED(FDyModelHandlerManager);
  MDY_SINGLETON_PROPERTIES(FDyModelHandlerManager);

  using TContainer = TStringHashMap<std::unique_ptr<DDyModelHandler>>;
  
  /// @brief 
  MDY_NODISCARD TContainer& GetContainer() noexcept;
  /// @brief 
  MDY_NODISCARD const TContainer& GetContainer() const noexcept;

  /// @brief Check there is bound model in container, using Model specifier name.
  MDY_NODISCARD bool IsBoundModelExist(_MIN_ const std::string& iModelSpecifier) const noexcept;

  /// @brief Try create handler, 
  /// given `iModelSpecifier` has a valid Reference Instance from `Resource` with `Model`.
  /// If not, this function will return DY_FAILURE.
  /// But If model specifier is exist in meta Information (not instanted), 
  /// just create and require resource and return DY_SUCCESS anyway.
  MDY_NODISCARD EDySuccess TryCreateHandler(_MIN_ const std::string& iModelSpecifier) noexcept;

  /// @brief
  EDySuccess BindToHandler(_MIN_ const std::string& iSpecifier, _MIN_ FDyActor& iRefActor, _MIN_ CModelFilter& iFilter);
  /// @brief
  EDySuccess BindToHandler(_MIN_ const std::string& iSpecifier, _MIN_ FDyActor& iRefActor, _MIN_ CModelRenderer& iComponent);
  /// @brief
  EDySuccess BindToHandler(_MIN_ const std::string& iSpecifier, _MIN_ FDyActor& iRefActor, _MIN_ CModelAnimator& iComponent);
  /// @brief
  EDySuccess UnbindToHandler(_MIN_ const std::string& iSpecifier, _MIN_ FDyActor& iRefActor, _MIN_ CModelFilter&);
  /// @brief
  EDySuccess UnbindToHandler(_MIN_ const std::string& iSpecifier, _MIN_ FDyActor& iRefActor, _MIN_ CModelRenderer&);
  /// @brief
  EDySuccess UnbindToHandler(_MIN_ const std::string& iSpecifier, _MIN_ FDyActor& iRefActor, _MIN_ CModelAnimator&);

  /// @brief 
  MDY_NODISCARD bool IsActorInfoNeedToBeGc(_MIN_ const std::string& iSpecifier, _MIN_ FDyActor& iRefActor) const noexcept;
  /// @brief 
  EDySuccess TryRemoveBoundActor(_MIN_ const std::string& iSpecifier, _MIN_ FDyActor& iRefActor);

  /// @brief This function checks valid `iSpecifier` item has no actor item. so can be deleted safely.
  MDY_NODISCARD bool IsBoundModelNeedToGc(_MIN_ const std::string& iSpecifier) const noexcept;
  /// @brief 
  EDySuccess TryRemoveBoundModel(_MIN_ const std::string& iSpecifier);

private:
  /// @brief 
  TContainer mModelHandlerContainer;
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_INTENRAL_RENDER_FDyModelHandlerManager_H