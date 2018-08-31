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

class DDyMaterialInformation final
{
public:
  DDyMaterialInformation(const PDyMaterialConstructionDescriptor& shaderConstructionDescriptor) :
      mMaterialInformation{shaderConstructionDescriptor}
  {};

  DDyMaterialInformation(const DDyMaterialInformation&)            = delete;
  DDyMaterialInformation& operator=(const DDyMaterialInformation&) = delete;
  DDyMaterialInformation(DDyMaterialInformation&&)            = default;
  DDyMaterialInformation& operator=(DDyMaterialInformation&&) = default;
  ~DDyMaterialInformation();

  ///
  /// @brief return immutable descriptor information reference.
  ///
  const PDyMaterialConstructionDescriptor& GetInformation() const noexcept
  {
    return this->mMaterialInformation;
  }

private:
  PDyMaterialConstructionDescriptor mMaterialInformation;

  void __pfSetNextLevel(CDyMaterialResource* ptr) const noexcept { mNextLevelPtr = ptr; }
  mutable CDyMaterialResource* mNextLevelPtr = nullptr;

  friend class CDyMaterialResource;
  friend class MDyResource;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_COMPONENT_INFORMATION_MATERIAL_INFORMATION_H