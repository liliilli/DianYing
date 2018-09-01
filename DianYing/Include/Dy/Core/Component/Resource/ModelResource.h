#ifndef GUARD_DY_CORE_COMPONENT_RESOURCE_MODEL_RESOURCE_H
#define GUARD_DY_CORE_COMPONENT_RESOURCE_MODEL_RESOURCE_H
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

#include <Dy/Core/Component/Resource/MeshResource.h>

//!
//! Forward declaration
//!

namespace dy
{
class DDyModelInformation;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

///
/// @class CDyModelResource
/// @brief
///
class CDyModelResource
{
public:
  CDyModelResource() = default;
  CDyModelResource(const CDyModelResource&)             = delete;
  CDyModelResource(CDyModelResource&&)                  = default;
  CDyModelResource& operator=(const CDyModelResource&)  = delete;
  CDyModelResource& operator=(CDyModelResource&&)       = default;
  ~CDyModelResource();

private:
  ///
  /// @brief
  ///
  [[nodiscard]]
  EDySuccess pInitializeModel(const DDyModelInformation& modelInformation);

  std::vector<std::unique_ptr<CDyMeshResource>> mMeshResource = {};

  //!
  //! Level pointers binding
  //!

  template <typename TType>
  using TBindPtrMap = std::unordered_map<TType*, TType*>;
  ///
  /// @brief
  ///
  void __pfSetPrevLevel(DDyModelInformation* ptr) const noexcept { __mPrevLevelPtr = ptr; }
  void __pfSetRendererBind(void* ptr) const noexcept
  {
    auto [it, result] = __mBindRendererPtrs.try_emplace(ptr, ptr);
    if (!result) {
      assert(false);
    }
  }
  void __pfSetRendererReset(void* ptr) const noexcept
  {
    __mBindRendererPtrs.erase(ptr);
  }
  mutable DDyModelInformation*  __mPrevLevelPtr     = nullptr;
  mutable TBindPtrMap<void>     __mBindRendererPtrs;

  friend class DDyModelInformation;
  friend class MDyResource;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_COMPONENT_RESOURCE_MODEL_RESOURCE_H