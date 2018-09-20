#ifndef GUARD_DY_CORE_COMPONENT_INFORMATION_MATERIAL_INFORMATION_H
#define GUARD_DY_CORE_COMPONENT_INFORMATION_MATERIAL_INFORMATION_H
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

#include <Dy/Core/Component/Internal/MaterialType.h>
#include <Dy/Helper/Pointer.h>
#include "Dy/Core/Component/Interface/IDyToString.h"

//!
//! Forward declaration
//!

namespace dy
{
class CDyMaterialResource;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

///
/// @class DDyMaterialInformation
/// @brief Information class for material information.
///
class DDyMaterialInformation final : public IDyToString
{
public:
  MDY_NOT_COPYABLE_MOVEABLE_PROPERTIES(DDyMaterialInformation);
  DDyMaterialInformation(const PDyMaterialConstructionDescriptor& materialConstructionDescriptor);
  ~DDyMaterialInformation();

  ///
  /// @brief return immutable descriptor information reference.
  ///
  [[nodiscard]]
  FORCEINLINE const PDyMaterialConstructionDescriptor& GetInformation() const noexcept
  {
    return this->mMaterialInformation;
  }

  ///
  /// @brief Check if object is being binded to CDyMaterialResource instance.
  ///
  [[nodiscard]]
  FORCEINLINE bool IsBeingBindedToResource() const noexcept
  {
    return this->__mLinkedMaterialResourcePtr != nullptr;
  }

  ///
  /// mMaterialInformation.ToString();
  ///
  [[nodiscard]]
  std::string ToString() override;

private:
  ///
  /// @brief Enroll populated material name and get index for avoiding name duplication of
  /// derived material name.
  ///
  [[nodiscard]]
  int32_t __pfEnrollAndGetNextDerivedMaterialIndex(const std::string& name) const noexcept;

  ///
  /// @brief Populate independent material reference and move ownership to outside.
  ///
  [[nodiscard]]
  std::unique_ptr<DDyMaterialInformation> __pfPopulateMaterialWith(const PDyMaterialPopulateDescriptor& desc) const noexcept;

  /// Information sturcture.
  PDyMaterialConstructionDescriptor mMaterialInformation;

  //!
  //! Resource pointers binding
  //!

  FORCEINLINE void __pfSetMaterialResourceLink(NotNull<CDyMaterialResource*> ptr) const noexcept
  {
    this->__mLinkedMaterialResourcePtr = ptr;
  }
  FORCEINLINE void __pfResetMaterialResourceLink() const noexcept
  {
    this->__mLinkedMaterialResourcePtr = nullptr;
  }

  MDY_TRANSIENT CDyMaterialResource*                      __mLinkedMaterialResourcePtr = nullptr;
  MDY_TRANSIENT std::unordered_map<std::string, int32_t>  __mPopulatedMaterialIndexMap = {};

  friend class CDyMaterialResource;
  friend class MDyDataInformation;
  friend class MDyHeapResource;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_COMPONENT_INFORMATION_MATERIAL_INFORMATION_H