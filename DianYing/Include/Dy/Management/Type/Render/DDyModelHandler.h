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
#include <Dy/Core/Resource/Type/TDyResourceBinder.h>

//!
//! Forward declaration
//!

namespace dy
{
class FDyActor;
class CDyModelFilter;
class CDyModelRenderer;
class CDyTransform;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

/// @struct DDyModelHandler
/// @brief Model handler.
struct DDyModelHandler final
{
public:
  /// @brief
  DDyModelHandler(_MIN_ const std::string& iModelSpecifier);

  /// @brief Check there is actor item.
  MDY_NODISCARD bool IsActorItemExist(_MIN_ FDyActor& iRefActor) const noexcept;
  /// @brief Check given actor item needs to be GCed.
  /// This function can not check null-ed instance.
  MDY_NODISCARD bool IsActorNeedToBeGc(_MIN_ FDyActor& iRefActor) const noexcept;
  /// @brief Check there is no actor information.
  MDY_NODISCARD bool IsEmpty() const noexcept; 

  /// @brief Try create actor binding information instance to container.
  /// If there is already existed one, just return DY_FAILURE.
  /// This function can not be called independenty
  EDySuccess TryCreateActorBinding(_MIN_ FDyActor& iRefActor);
  /// @brief 
  EDySuccess TryRemoveActorBinding(_MIN_ FDyActor& iRefActor);
  
  /// @brief Bind filter. If Actor information item is not exist yet, just return DY_FAILURE.
  EDySuccess BindFilter(_MIN_ FDyActor& iRefActor, _MIN_ CDyModelFilter& iRefFilter);
  /// @brief Unbind filter.
  EDySuccess UnbindFilter(_MIN_ FDyActor& iRefActor);

private:
  /// @struct DActorInfo
  /// @brief 
  struct DActorInfo final
  {
    CDyModelFilter*   mPtrCompModelFilter = nullptr;
    CDyModelRenderer* mPtrModelRenderer   = nullptr;
  };

  /// Only specifies one valid model.
  TDyLResourceBinderModel                   mModelBinder;
  /// Have pointer of actor and components information.
  std::unordered_map<FDyActor*, DActorInfo> mActorInformation;
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_TYPE_RENDER_DDyModelHandler_H