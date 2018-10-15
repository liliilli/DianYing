#ifndef GUARD_DY_COMPONENT_CDYMODELRENDERER_H
#define GUARD_DY_COMPONENT_CDYMODELRENDERER_H
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

#include <Dy/Element/Abstract/ADyGeneralBaseComponent.h>
#include <Dy/Component/Interface/IDyInitializeHelper.h>
#include <Dy/Component/Descriptor/ComponentMetaDescriptor.h>
#include <Dy/Helper/Internal/CheckingRule.h>
#include <Dy/Core/Component/Resource/MaterialResource.h>

//!
//! Forward declaration
//!

namespace dy
{

class CDyModelFilter;

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
class CDyModelRenderer final : public ADyGeneralBaseComponent, public IDyInitializeHelper<DDyModelRendererMetaInformation>
{
public:
  CDyModelRenderer(FDyActor& actorReference);
  virtual ~CDyModelRenderer() = default;

  CDyModelRenderer(const CDyModelRenderer&)                                 = delete;
  CDyModelRenderer& operator=(const CDyModelRenderer&)                      = delete;
  CDyModelRenderer(CDyModelRenderer&& instance)                   noexcept  = default;
  CDyModelRenderer& operator=(CDyModelRenderer&& instance)        noexcept  = default;

  ///
  /// @brief  Initialize with descriptor.
  /// @param  descriptor
  /// @return If successful, return DY_SUCCESS or DY_FAILURE.
  ///
  MDY_NODISCARD EDySuccess Initialize(const DDyModelRendererMetaInformation& descriptor) override final;

  /// Release component.
  void Release() override final;

  ///
  /// @brief Pingpong.
  /// @param validReference
  ///
  void BindModelFilterReference(CDyModelFilter& validReference);

  ///
  /// @brief Pingpong.
  /// @param
  ///
  void UnbindModelFilterReference();

  /// @brief Update sequence etc.
  void Update(float dt) override final {};

  /// @brief Request MDyWorld to draw this instance.
  void RequestDrawCall() noexcept;

  /// @brief Activate component.
  void Activate() noexcept override final;

  /// @brief Deactivate component.
  void Deactivate() noexcept override final;

  ///
  /// @brief  Get the element count of binded material list.
  /// @return The number of material elements binded to CDyModelRenderer::mMaterialResourcePtr.
  ///
  MDY_NODISCARD FORCEINLINE TI32 GetMaterialListCount() const noexcept
  {
    return static_cast<TI32>(this->mMaterialResourcePtr.size());
  }

  ///
  /// @brief  Get the number of submesh of model instance which binded to this component instance.
  /// @return The number of submesh elements binded to CDyModelRenderer::mModelFilter.
  ///
  MDY_NODISCARD std::optional<TI32> GetModelSubmeshCount() const noexcept;

  ///
  /// @brief
  /// @param  index
  /// @return
  ///
  const CDyMaterialResource& GetMaterialResourcePtr(_MIN_ const TI32 index) const noexcept;

  ///
  /// @brief
  /// @param  index
  /// @return
  ///
  const CDySubmeshResource& GetSubmeshResourcePtr(_MIN_ const TI32 index) const noexcept;

  ///
  /// @brief
  /// @param actorBool
  ///
  void pPropagateParentActorActivation(const DDy3StateBool& actorBool) noexcept override final;

  ///
  /// @brief
  ///
  MDY_NODISCARD std::string ToString() override final;

private:
  ///
  /// @brief
  /// @return
  ///
  MDY_NODISCARD EDySuccess pTryBindingToModelFilterComponent();

  ///
  /// @brief
  /// @return
  ///
  MDY_NODISCARD EDySuccess pTryUnbindingToModelFilterComponent();

  /// Valid model filter pointer reference. If value is nullptr, do not use.
  CDyModelFilter* mModelFilter            = MDY_INITIALIZE_NULL;
  /// Check flag for create shadow or not.
  bool            mIsEnabledCreateShadow  = false;
  /// @TODO TEMPORAL MAYBE?
  std::vector<CDyMaterialResource*> mMaterialResourcePtr  = {};

  MDY_SET_TYPEMATCH_FUNCTION(::dy::ADyGeneralBaseComponent, CDyModelRenderer);
  MDY_SET_CRC32_HASH_WITH_TYPE(CDyModelRenderer);
};

MDY_TEST_FULFILLS_MOVE_ONLY(CDyModelRenderer);

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_CDYMODELRENDERER_H