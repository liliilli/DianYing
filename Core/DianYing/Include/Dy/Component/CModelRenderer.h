#ifndef GUARD_DY_COMPONENT_CDYMODELRENDERER_H
#define GUARD_DY_COMPONENT_CDYMODELRENDERER_H
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

#include <Dy/Core/Resource/Type/TResourceBinder.h>
#include <Dy/Component/Interface/IInitializeHelper.h>
#include <Dy/Element/Abstract/AGeneralBaseComponent.h>
#include <Dy/Helper/Internal/XCheckingRule.h>
#include <Dy/Meta/Information/ComponentMetaInformation.h>

//!
//! Forward declaration
//!

namespace dy
{
class FResourceMaterial;
class CModelFilter;
class FResourceMesh;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

/// @class CModelRenderer
/// @brief Component for rendering model from CModelFilter
class CModelRenderer final : public AGeneralBaseComponent, public IInitializeHelper<PDyModelRendererComponentMetaInfo>
{
public:
  CModelRenderer(FActor& actorReference);
  virtual ~CModelRenderer() = default;

  CModelRenderer(const CModelRenderer&)            = delete;
  CModelRenderer& operator=(const CModelRenderer&) = delete;
  CModelRenderer(CModelRenderer&& instance)            noexcept = default;
  CModelRenderer& operator=(CModelRenderer&& instance) noexcept = default;

  /// @brief  Initialize with descriptor.
  /// @param  descriptor
  /// @return If successful, return EDySuccess::DY_SUCCESS or EDySuccess::DY_FAILURE.
  MDY_NODISCARD EDySuccess Initialize(const PDyModelRendererComponentMetaInfo& descriptor) override final;

  /// Release component.
  void Release() override final;

  /// @brief Pingpong.
  /// @param validReference
  void BindModelFilterReference(CModelFilter& validReference);

  /// @brief Pingpong.
  /// @param
  void UnbindModelFilterReference();

  /// @brief Update sequence etc.
  void Update(float dt) override final {};

  /// @brief  Get the number of submesh of model instance which binded to this component instance.
  /// @return The number of submesh elements binded to CModelRenderer::mPtrModelFilterComponent.
  MDY_NODISCARD std::optional<TI32> GetModelSubmeshCount() const noexcept;

  /// @brief
  /// @param  index
  /// @return
  const FResourceMaterial& GetMaterialResourcePtr(TU32 index) const noexcept;

  /// @brief
  /// @param  index
  /// @return
  const FResourceMesh& GetSubmeshResourcePtr(TU32 index) const noexcept;
  
  /// @brief Get model resource binder. If renderer is not binding any Model, just return nullptr.
  MDY_NODISCARD TDyResourceBinderModel* GetModelResourceBinder() noexcept;
 
  /// @brief Get model resource binder. If renderer is not binding any Model, just return nullptr.
  MDY_NODISCARD const TDyResourceBinderModel* GetModelResourceBinder() const noexcept;

  /// @brief
  MDY_NODISCARD std::string ToString() override final;

private:
  void TryActivateInstance() override final;
  void TryDeactivateInstance() override final;

  /// Valid model filter pointer reference. If value is nullptr, do not use.
  CModelFilter* mPtrModelFilterComponent  = MDY_INITIALIZE_NULL;
  /// Check flag for create shadow or not.
  bool            mIsEnabledCreateShadow    = false;

  MDY_SET_TYPEMATCH_FUNCTION(::dy::AGeneralBaseComponent, CModelRenderer);
  MDY_SET_CRC32_HASH_WITH_TYPE(CDyModelRenderer);
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_CDYMODELRENDERER_H