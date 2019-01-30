#ifndef GUARD_DY_COMPONENT_CDYMODELFILTER_H
#define GUARD_DY_COMPONENT_CDYMODELFILTER_H
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

#include <Dy/Element/Abstract/ADyGeneralBaseComponent.h>
#include <Dy/Core/Resource/Type/TDyResourceBinder.h>
#include <Dy/Component/Interface/IDyInitializeHelper.h>
#include <Dy/Meta/Information/ComponentMetaInformation.h>
#include <Dy/Helper/Pointer.h>

//!
//! Forward decalartion
//!

namespace dy
{
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

  MDY_ONLY_MOVEABLE_PROPERTIES_DEFAULT(CDyModelFilter);
  MDY_SET_TYPEMATCH_FUNCTION(::dy::ADyGeneralBaseComponent, CDyModelFilter);
  MDY_SET_CRC32_HASH_WITH_TYPE(CDyModelFilter);

  /// @brief Get model reference ptr.
  /// @return Valid model resource pointer reference.
  MDY_NODISCARD const TDyLResourceBinderModel& GetModelReference() const noexcept;

  /// @brief Get model reference ptr.
  MDY_NODISCARD TDyLResourceBinderModel& GetModelReference() noexcept;

  /// @brief Get model specifier name.
  MDY_NODISCARD const std::string& GetModelSpecifier() const noexcept;

  ///
  /// @brief Bind valid CDyModelRenderer pointer from same FDyActor to this component.
  /// @param validReference valid CDyModelReference instance.
  ///
  void fBindModelRendererReference(CDyModelRenderer& validReference);

  /// @brief Unbind valid CDyModelRenderer pointer to null.
  void fUnbindModelRendererReference();

  /// @brief Do nothing.
  void Update(float dt) override final {};

  ///
  /// @brief  Get information string of CDyModelFilter instance.
  /// @return Information string of CDyModelFilter.
  ///
  MDY_NODISCARD std::string ToString() override final;

private:
  /// @brief This function must be implemented. 
  /// This function will be called when `Activate()` or `SetupFlagAsParent` called, 
  /// and final state boolean value is true from false.
  void TryActivateInstance() override final;
  
  /// @brief This function must be implemented.
  /// This function will be called when `Deactivate()` or `SetupFlagAsParent(false)` called,
  /// and final state boolean value is false from true.
  void TryDeactivateInstance() override final;

  /// Valid model reference handle.
  TDyLResourceBinderModel mBinderModel = {};
  /// Model specifier name for activate & deactivating.
  std::string             mModelSpecifier = MDY_INITIALIZE_EMPTYSTR;
  /// CDyModelRendererr reference ptr.
  CDyModelRenderer*       mModelRendererReferencePtr  = MDY_INITIALIZE_NULL;

  friend class CDyModelRenderer;
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_CDYMODELFILTER_H