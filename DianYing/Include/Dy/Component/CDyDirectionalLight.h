#ifndef GUARD_DY_COMPONENT_CDYDIRECTIONALLIGHT_H
#define GUARD_DY_COMPONENT_CDYDIRECTIONALLIGHT_H
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
#include <Dy/Component/Descriptor/ComponentMetaDescriptor.h>
#include <Dy/Component/Interface/IDyInitializeHelper.h>

namespace dy
{

///
/// @struct DDyUboDirectionalLight
/// @brief  This structure must be aligned by 16 bytes. (for std140 GL standard block layout)
/// @TODO SCRIPT THIS
///
struct alignas(16) DDyUboDirectionalLight final
{
  /// World space light direction
  DDyVector3  mDirection  = {};
  MDY_NOTUSED TI32 ____padding;
  /// Light tint color
  DDyColor    mDiffuse    = DDyColor::White;
  DDyColor    mSpecular   = DDyColor::White;
  DDyColor    mAmbient    = DDyColor::White;
  /// Light intensity for this light component.
  float       mIntensity  = MDY_INITIALIZE_DEFINT;
};


///
/// @struct CDyDirectionalLight
/// @brief
/// @TODO SCRIPT THIS
///
class CDyDirectionalLight final : public ADyGeneralBaseComponent, public IDyInitializeHelper<DDyDirectionalLightMetaInformation>
{
public:
  CDyDirectionalLight(FDyActor& actorReference);
  virtual ~CDyDirectionalLight() = default;

  ///
  /// @brief  Initialize component
  /// @param  metaInfo
  /// @return If successful just return DY_SUCCESS or DY_FAILURE.
  ///
  MDY_NODISCARD EDySuccess Initialize(_MIN_ const DDyDirectionalLightMetaInformation& metaInfo) override final;

  ///
  /// @brief  Release component.
  ///
  void Release() override final;

  CDyDirectionalLight(const CDyDirectionalLight&)                                 = delete;
  CDyDirectionalLight& operator=(const CDyDirectionalLight&)                      = delete;
  CDyDirectionalLight(CDyDirectionalLight&& instance)                   noexcept  = default;
  CDyDirectionalLight& operator=(CDyDirectionalLight&& instance)        noexcept  = default;

  MDY_SET_TYPEMATCH_FUNCTION(::dy::ADyGeneralBaseComponent, CDyDirectionalLight);
  MDY_SET_CRC32_HASH_WITH_TYPE(CDyDirectionalLight)

  /// @brief Do nothing.
  void Update(float dt) override final;

  /// @brief Activate CDyDirectionalLight. Final activation value is also dependent on FDyActor activation flag.
  void Activate() noexcept override final;

  /// @brief Dectivate CDyDirectionalLight. Final activation value is also dependent on FDyActor activation flag.
  void Deactivate() noexcept override final;

  ///
  /// @brief
  /// @param
  /// @TODO SCRIPT THIS
  ///
  void pPropagateParentActorActivation(const DDy3StateBool& actorBool) noexcept override final;

  ///
  /// @brief
  /// @return
  /// @TODO SCRIPT THIS
  ///
  MDY_NODISCARD std::string ToString() override final;

  ///
  /// @brief
  /// @return Casting light
  /// @TODO SCRIPT THIS
  ///
  FORCEINLINE MDY_NODISCARD bool IsCastingLight() const noexcept { return this->mIsCastingLight; }

  ///
  /// @brief
  /// @return
  /// @TODO SCRIPT THIS
  ///
  FORCEINLINE MDY_NODISCARD bool IsCastingShadow() const noexcept { return this->mIsCastingShadow; }

  ///
  /// @brief
  /// @param flag
  ///
  void SetCastingLightFlag(_MIN_ const bool flag) noexcept;

  ///
  /// @brief
  /// @param flag
  ///
  void SetCastingShadowFlag(_MIN_ const bool flag) noexcept;

  ///
  /// @brief
  /// @return
  /// @TODO SCRIPT THIS
  ///
  FORCEINLINE MDY_NODISCARD const DDyVector3& GetLightDirection() const noexcept
  {
    return this->mData.mDirection;
  }

  ///
  /// @brief
  /// @return
  /// @TODO SCRIPT THIS
  ///
  FORCEINLINE MDY_NODISCARD float GetLightIntensity() const noexcept
  {
    return this->mData.mIntensity;
  }

  ///
  /// @brief
  /// @return
  /// @TODO SCRIPT THIS
  ///
  FORCEINLINE MDY_NODISCARD const DDyColor& GetLightDiffuseColor() const noexcept
  {
    return this->mData.mDiffuse;
  }

  ///
  /// @brief
  /// @param direction
  /// @TODO SCRIPT THIS
  ///
  FORCEINLINE void SetLightDirection(_MIN_ const DDyVector3& direction) noexcept
  {
    this->mData.mDirection = direction.Normalize();
    this->mIsNeededUpdateValueToGpu = true;
  }

  ///
  /// @brief
  /// @param intensity
  /// @TODO SCRIPT THIS
  ///
  void SetIntensity(_MIN_ const float intensity) noexcept
  {
    if (intensity < 0.f)  { this->mData.mIntensity = 0.001f; }
    else                  { this->mData.mIntensity = intensity; }
    this->mIsNeededUpdateValueToGpu = true;
  }

  ///
  /// @brief
  /// @param color
  /// @TODO SCRIPT THIS
  ///
  FORCEINLINE void SetLightDiffuseColor(_MIN_ const DDyColor& color) noexcept
  {
    this->mData.mDiffuse = color;
    this->mIsNeededUpdateValueToGpu = true;
  }

  ///
  /// @brief
  /// @return If binded to lighting system now, return true.
  /// @TODO SCRIPT THIS
  ///
  FORCEINLINE MDY_NODISCARD bool IsBindedToLightingSystem() const noexcept
  {
    return this->mCastingLightUboIndex != MDY_INITIALIZE_DEFINT;
  }

  ///
  /// @brief
  /// @return
  /// @TODO SCRIPT THIS
  ///
  MDY_NODISCARD std::optional<TI32> TryGetBindedIndexValue() const noexcept
  {
    if (this->mCastingLightUboIndex == MDY_INITIALIZE_DEFINT) { return std::nullopt; }
    else                                                      { return this->mCastingLightUboIndex; }
  }

private:
  ///
  /// @brief Try activate camera operation. \n
  /// If parent is activate and itself activated, update all properties newly.
  /// @return
  /// @TODO SCRIPT THIS
  ///
  MDY_NODISCARD EDySuccess pTryActivateDirectionalLight();

  ///
  /// @brief
  /// @return
  /// @TODO SCRIPT THIS
  ///
  MDY_NODISCARD EDySuccess pTryActivateCastingShadow();

  ///
  /// @brief Try deactivate camera operation. \n
  /// If either parent or itself is deactivated, disconnect it with all outside system.
  /// @return
  /// @TODO SCRIPT THIS
  ///
  MDY_NODISCARD EDySuccess pTryDeactivateDirectionalLight();

  ///
  /// @brief
  /// @return
  /// @TODO SCRIPT THIS
  ///
  MDY_NODISCARD EDySuccess pTryDeactivateCastingShadow();

  ///
  /// @brief
  /// @return
  /// @TODO SCRIPT THIS
  ///
  MDY_NODISCARD EDySuccess pTryUpdateDirectionalLight();

  ///
  DDyUboDirectionalLight  mData           = {};
  ///
  MDY_NOTUSED bool        mIsCastingLight  = false;
  ///
  MDY_NOTUSED bool        mIsCastingShadow = false;

  ///
  TI32        mCastingLightUboIndex = MDY_INITIALIZE_DEFINT;
  ///
  bool        mIsBindedToRenderingManagerAsLighting = false;
  ///
  bool        mIsBindedToRenderingManagerAsShadow = false;
  /// Flag for updating new value to gpu memory buffer.
  bool        mIsNeededUpdateValueToGpu = true;
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_CDYDIRECTIONALLIGHT_H