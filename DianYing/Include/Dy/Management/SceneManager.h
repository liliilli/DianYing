#ifndef GUARD_DY_CORE_COMPONENT_RESOURCE_SCENE_MANAGER_H
#define GUARD_DY_CORE_COMPONENT_RESOURCE_SCENE_MANAGER_H
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

#include <Dy/Element/Interface/IDyUpdatable.h>
#include <Dy/Management/Interface/ISingletonCrtp.h>
#include "Dy/Element/Level.h"

//!
//! Forward declaration
//!

namespace dy
{
class CDyCamera;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

///
/// @class MDyScene
/// @brief Manages scene and helping access each scene's unique properties.
///
class MDyScene final : public ISingleton<MDyScene>, public IDyUpdatable
{
  MDY_SINGLETON_DERIVED(MDyScene);
  MDY_SINGLETON_PROPERTIES(MDyScene);
public:
  ///
  /// @brief Update scene structures prior to dive in scene objects tree hierarchy.
  /// Scene transition will be executed maybe.
  ///
  void Update(float dt) override final;

  ///
  /// @brief Update valid objects. this function must be called after this->Update().
  ///
  void UpdateObjects(float dt);

  ///
  /// @brief Return main camera ptr.
  ///
  [[nodiscard]] CDyCamera* GetMainCameraPtr() const noexcept;

  ///
  /// @brief Check if main camera is binded so be able to focused by scene.
  ///
  [[nodiscard]] FORCEINLINE bool IsMainCameraFocused() const noexcept
  {
    return this->mValidMainCameraPtr != nullptr;
  }

  ///
  /// @brief Ask it for opening level with levelName next frame.
  /// @param levelName valid level meta information name
  ///
  EDySuccess OpenLevel(const std::string& levelName);

private:
  /// Bind valid camera to main camera and let object have focused.
  void __pfBindFocusCamera(CDyCamera* validCameraPtr);
  /// Unbind main camera. this function must not be called manually, but using camera's mechanism.
  void __pfUnbindCameraFocus();

  /// @brief
  CDyCamera*              mValidMainCameraPtr = nullptr;
  std::vector<CDyCamera*> mValidSubCameraPtrs = {};

  std::string             mNextLevelName      = MDY_NOT_INITILAIZED_STR;
  std::string             mPresentLevelName   = MDY_NOT_INITILAIZED_STR;
  std::string             mPreviousLevelName  = MDY_NOT_INITILAIZED_STR;

  std::unique_ptr<FDyLevel> mLevel            = nullptr;

  friend class CDyCamera;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_COMPONENT_RESOURCE_SCENE_MANAGER_H