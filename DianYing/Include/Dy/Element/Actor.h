#ifndef GUARD_DY_ELEMENT_ACTOR_H
#define GUARD_DY_ELEMENT_ACTOR_H
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

#include <Dy/Element/Object.h>
#include <Dy/Element/Abstract/ADyTransformable.h>
#include <Dy/Core/Component/Internal/MaterialType.h>

namespace dy
{

///
/// @class FDyActor
/// @brief FFF
///
class FDyActor : public FDyObject, public ADyTransformable
{
public:
  FDyActor() = default;

  ///
  /// @brief Get present actor name on runtime.
  /// @return Actor name of this instance.
  ///
  [[nodiscard]] const std::string& GetActorName() const noexcept
  {
    return this->pGetObjectName();
  }

  ///
  /// @brief Set FDyActor's hierarchial position to Parent's children FDyActor.
  /// Transform update will be held automatically.
  ///
  /// @param validParentRawPtr
  ///
  void SetParent(NotNull<FDyActor*> validParentRawPtr) noexcept;

  ///
  /// @brief
  ///
  /// @param
  ///
  void SetParentRelocateTransform(NotNull<FDyActor*> validParentRawPtr) noexcept;

  ///
  /// @brief Set FDyActor's hierarchial position to Scene's root.
  /// Transform update will be held automatically.
  ///
  void SetParentToRoot() noexcept;

  ///
  /// @brief
  ///
  void SetParentToRootRelocateTransform() noexcept;

  ///
  std::string ToString() override
  {
    return MDY_NOT_INITILAIZED_STR;
  }

private:
  /// Parent FDyActor raw-pointer data.
  FDyActor* mParentFDyActorRawPtr = nullptr;
};

} /// ::dy namespace

#endif /// GUARD_DY_ELEMENT_ACTOR_H