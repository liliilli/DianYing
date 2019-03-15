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

#include <Dy/Element/Abstract/AGeneralBaseComponent.h>
#include <Dy/Core/Resource/Type/TResourceBinder.h>
#include <Dy/Component/Interface/IInitializeHelper.h>
#include <Dy/Meta/Information/ComponentMetaInformation.h>
#include <Dy/Helper/System/Pointer.h>

//!
//! Forward decalartion
//!

namespace dy
{
class CModelRenderer;
} /// unnamed namespace

//!
//! Implementation
//!

namespace dy
{

/// @class CModelFilter
/// @brief The component that binds model information to actor.
class CModelFilter final : public AGeneralBaseComponent, public IInitializeHelper<PDyModelFilterComponentMetaInfo>
{
public:
  CModelFilter(FActor& actorReference);
  virtual ~CModelFilter() = default;

  /// @brief  Initialize component
  /// @param  metaInfo
  /// @return If successful just return DY_SUCCESS or DY_FAILURE.
  MDY_NODISCARD EDySuccess Initialize(const PDyModelFilterComponentMetaInfo& metaInfo) override final;

  /// @brief  Release component.
  void Release() override final;

  MDY_ONLY_MOVEABLE_PROPERTIES_DEFAULT(CModelFilter);
  MDY_SET_TYPEMATCH_FUNCTION(::dy::AGeneralBaseComponent, CModelFilter);
  MDY_SET_CRC32_HASH_WITH_TYPE(CModelFilter);

  /// @brief Get model reference ptr.
  /// @return Valid model resource pointer reference.
  MDY_NODISCARD const TDyResourceBinderModel& GetModelReference() const noexcept;

  /// @brief Get model reference ptr.
  MDY_NODISCARD TDyResourceBinderModel& GetModelReference() noexcept;

  /// @brief Get model specifier name.
  MDY_NODISCARD const std::string& GetModelSpecifier() const noexcept;

  /// @brief Bind valid CModelRenderer pointer from same FActor to this component.
  /// @param validReference valid CDyModelReference instance.
  void fBindModelRendererReference(CModelRenderer& validReference);

  /// @brief Unbind valid CModelRenderer pointer to null.
  void fUnbindModelRendererReference();

  /// @brief Do nothing.
  void Update(float dt) override final {};

  /// @brief  Get information string of CModelFilter instance.
  /// @return Information string of CModelFilter.
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
  TDyResourceBinderModel mBinderModel = {};
  /// Model specifier name for activate & deactivating.
  std::string             mModelSpecifier = MDY_INITIALIZE_EMPTYSTR;
  /// CDyModelRendererr reference ptr.
  CModelRenderer*       mModelRendererReferencePtr  = MDY_INITIALIZE_NULL;

  friend class CModelRenderer;
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_CDYMODELFILTER_H