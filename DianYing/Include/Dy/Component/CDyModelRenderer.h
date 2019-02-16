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

#include <Dy/Core/Resource/Type/TDyResourceBinder.h>
#include <Dy/Component/Interface/IDyInitializeHelper.h>
#include <Dy/Element/Abstract/ADyGeneralBaseComponent.h>
#include <Dy/Helper/Internal/CheckingRule.h>
#include <Dy/Meta/Information/ComponentMetaInformation.h>

//!
//! Forward declaration
//!

namespace dy
{
class FDyMaterialResource;
class CDyModelFilter;
class FDyMeshResource;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

///
/// @class CDyModelRenderer
/// @brief
///
class CDyModelRenderer final : public ADyGeneralBaseComponent, public IDyInitializeHelper<PDyModelRendererComponentMetaInfo>
{
public:
  CDyModelRenderer(FDyActor& actorReference);
  virtual ~CDyModelRenderer() = default;

  CDyModelRenderer(const CDyModelRenderer&)                                 = delete;
  CDyModelRenderer& operator=(const CDyModelRenderer&)                      = delete;
  CDyModelRenderer(CDyModelRenderer&& instance)                   noexcept  = default;
  CDyModelRenderer& operator=(CDyModelRenderer&& instance)        noexcept  = default;

  /// @brief  Initialize with descriptor.
  /// @param  descriptor
  /// @return If successful, return DY_SUCCESS or DY_FAILURE.
  MDY_NODISCARD EDySuccess Initialize(const PDyModelRendererComponentMetaInfo& descriptor) override final;

  /// Release component.
  void Release() override final;

  /// @brief Pingpong.
  /// @param validReference
  void BindModelFilterReference(CDyModelFilter& validReference);

  /// @brief Pingpong.
  /// @param
  void UnbindModelFilterReference();

  /// @brief Update sequence etc.
  void Update(float dt) override final {};

  /// @brief  Get the number of submesh of model instance which binded to this component instance.
  /// @return The number of submesh elements binded to CDyModelRenderer::mPtrModelFilterComponent.
  MDY_NODISCARD std::optional<TI32> GetModelSubmeshCount() const noexcept;

  /// @brief
  /// @param  index
  /// @return
  const FDyMaterialResource& GetMaterialResourcePtr(_MIN_ TU32 index) const noexcept;

  /// @brief
  /// @param  index
  /// @return
  const FDyMeshResource& GetSubmeshResourcePtr(_MIN_ TU32 index) const noexcept;
  
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
  CDyModelFilter* mPtrModelFilterComponent  = MDY_INITIALIZE_NULL;
  /// Check flag for create shadow or not.
  bool            mIsEnabledCreateShadow    = false;

  MDY_SET_TYPEMATCH_FUNCTION(::dy::ADyGeneralBaseComponent, CDyModelRenderer);
  MDY_SET_CRC32_HASH_WITH_TYPE(CDyModelRenderer);
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_CDYMODELRENDERER_H