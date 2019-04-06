#ifndef GUARD_DY_MANAGEMENT_TYPE_RENDER_DDyModelHandler_H
#define GUARD_DY_MANAGEMENT_TYPE_RENDER_DDyModelHandler_H
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

#include <unordered_map>
#include <Dy/Core/Resource/Type/TResourceBinder.h>

//!
//! Forward declaration
//!

namespace dy
{
class FActor;
class CModelFilter;
class CModelRenderer;
class CModelAnimator;
class CTransform;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

/// @struct DDyModelHandler
/// @brief Model handler for model rendering.
struct DDyModelHandler final
{
public:
  /// @struct DActorInfo
  /// @brief 
  struct DActorInfo final
  {
    CModelFilter*   mPtrCompModelFilter = nullptr;
    CModelRenderer* mPtrModelRenderer   = nullptr;
    CModelAnimator* mPtrModelAnimator   = nullptr;
  };
  using TContainer = std::unordered_map<FActor*, DActorInfo>;

  /// @brief
  DDyModelHandler(_MIN_ const std::string& iModelSpecifier);

  /// @brief Check there is actor item.
  MDY_NODISCARD bool IsActorItemExist(_MIN_ FActor& iRefActor) const noexcept;
  /// @brief Check given actor item needs to be GCed.
  /// This function can not check null-ed instance.
  MDY_NODISCARD bool IsActorNeedToBeGc(_MIN_ FActor& iRefActor) const noexcept;
  /// @brief Check there is no actor information.
  MDY_NODISCARD bool IsEmpty() const noexcept; 
  /// @brief Check resource binder is actaully valid.
  MDY_NODISCARD bool IsModelResourceValid() const noexcept;

  /// @brief Try create actor binding information instance to container.
  /// If there is already existed one, just return DY_FAILURE.
  /// This function can not be called independenty
  EDySuccess TryCreateActorBinding(_MIN_ FActor& iRefActor);
  /// @brief 
  EDySuccess TryRemoveActorBinding(_MIN_ FActor& iRefActor);
  
  /// @brief Bind filter. If Actor information item is not exist yet, just return DY_FAILURE.
  EDySuccess BindFilter(_MIN_ FActor& iRefActor, _MIN_ CModelFilter& iRefFilter);
  /// @brief Bind renderer. If Actor information item is not exist yet, just return DY_FAILURE.
  EDySuccess BindRenderer(_MIN_ FActor& iRefActor, _MIN_ CModelRenderer& iRefComp);
  /// @brief Bind animator. If Actor information item is not exist yet, just return DY_FAILURE.
  EDySuccess BindAnimator(_MIN_ FActor& iRefActor, _MIN_ CModelAnimator& iRefComp);
  /// @brief Unbind filter.
  EDySuccess UnbindFilter(_MIN_ FActor& iRefActor);
  /// @brief Unbind renderer.
  EDySuccess UnbindRenderer(_MIN_ FActor& iRefActor);
  /// @brief Unbind animator.
  EDySuccess UnbindAnimator(_MIN_ FActor& iRefActor);

  /// @brief
  MDY_NODISCARD const TContainer& GetActorContainer() const noexcept;
  /// @brief
  MDY_NODISCARD TContainer& GetActorContainer() noexcept;

  /// @brief
  MDY_NODISCARD const TDyResourceBinderModel& GetModelBinderReference() const noexcept;

private:
  /// Only specifies one valid model.
  TDyResourceBinderModel mModelBinder;
  /// Have pointer of actor and components information.
  TContainer mActorInformation;
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_TYPE_RENDER_DDyModelHandler_H