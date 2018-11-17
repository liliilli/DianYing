#ifndef GUARD_DY_COMPONENT_CDYMODELFILTER_H
#define GUARD_DY_COMPONENT_CDYMODELFILTER_H
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
#include <Dy/Meta/Information/ComponentMetaInformation.h>
#include <Dy/Helper/Pointer.h>

//!
//! Forward decalartion
//!

namespace dy
{
class CDyModelResource;
class CDyModelRenderer;
} /// unnamed namespace

//!
//! Implementation
//!

namespace dy
{

///
/// @class CDyModelFilter
/// @brief
///
class CDyModelFilter final : public ADyGeneralBaseComponent, public IDyInitializeHelper<PDyModelFilterComponentMetaInfo>
{
public:
  CDyModelFilter(FDyActor& actorReference);
  virtual ~CDyModelFilter() = default;

  ///
  /// @brief  Initialize component
  /// @param  metaInfo
  /// @return If successful just return DY_SUCCESS or DY_FAILURE.
  ///
  MDY_NODISCARD EDySuccess Initialize(_MIN_ const PDyModelFilterComponentMetaInfo& metaInfo) override final;

  ///
  /// @brief  Release component.
  ///
  void Release() override final;

  CDyModelFilter(const CDyModelFilter&)                                 = delete;
  CDyModelFilter& operator=(const CDyModelFilter&)                      = delete;
  CDyModelFilter(CDyModelFilter&& instance)                   noexcept  = default;
  CDyModelFilter& operator=(CDyModelFilter&& instance)        noexcept  = default;

  MDY_SET_TYPEMATCH_FUNCTION(::dy::ADyGeneralBaseComponent, CDyModelFilter);
  MDY_SET_CRC32_HASH_WITH_TYPE(CDyModelFilter);

  ///
  /// @brief  Get model reference ptr.
  /// @return Valid model resource pointer reference.
  ///
  MDY_NODISCARD FORCEINLINE NotNull<CDyModelResource*> GetModelReference() const noexcept
  {
    return DyMakeNotNull(this->mModelReferencePtr);
  }

  ///
  /// @brief Bind valid CDyModelRenderer pointer from same FDyActor to this component.
  /// @param validReference valid CDyModelReference instance.
  ///
  void fBindModelRendererReference(CDyModelRenderer& validReference);

  /// @brief Unbind valid CDyModelRenderer pointer to null.
  void fUnbindModelRendererReference();

  /// @brief Do nothing.
  void Update(float dt) override final {};

  /// @brief Activate CDyModelFilter. Final activation value is also dependent on FDyActor activation flag.
  void Activate() noexcept override final;

  /// @brief Dectivate CDyModelFilter. Final activation value is also dependent on FDyActor activation flag.
  void Deactivate() noexcept override final;

  ///
  /// @brief
  /// @param
  ///
  void pPropagateParentActorActivation(const DDy3StateBool& actorBool) noexcept override final;

  ///
  /// @brief  Get information string of CDyModelFilter instance.
  /// @return Information string of CDyModelFilter.
  ///
  MDY_NODISCARD std::string ToString() override final;

private:
  ///
  MDY_NODISCARD EDySuccess pTryBindingToModelRendererComponent();

  ///
  MDY_NODISCARD EDySuccess pTryUnbindingToModelRendererComponent();

  /// Valid model rerenfence ptr.
  MDY_TRANSIENT CDyModelResource* mModelReferencePtr          = MDY_INITIALIZE_NULL;
  /// CDyModelRendererr reference ptr.
  CDyModelRenderer*               mModelRendererReferencePtr  = MDY_INITIALIZE_NULL;

  friend class CDyModelRenderer;
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_CDYMODELFILTER_H