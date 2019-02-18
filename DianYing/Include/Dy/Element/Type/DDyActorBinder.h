#ifndef GUARD_DY_ELEMENT_TYPE_DDYACTORBINDER_H
#define GUARD_DY_ELEMENT_TYPE_DDYACTORBINDER_H
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

//!
//! Forward declaration
//!

namespace dy
{
class   FDyActor;
struct  PDyActorCreationDescriptor;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

class DDyActorBinder final
{
public:
  DDyActorBinder() = default;
  ~DDyActorBinder() noexcept;
  DDyActorBinder(_MIN_ const DDyActorBinder& mRefBinder) noexcept;
  DDyActorBinder& operator=(_MIN_ const DDyActorBinder& mRefBinder) noexcept;

  /// @brief Check actor is created.
  MDY_NODISCARD bool IsActorCreated() const noexcept;

  FDyActor*       operator->() noexcept       { return this->mPtrActor; }
  const FDyActor* operator->() const noexcept { return this->mPtrActor; }

  void MDY_PRIVATE(BindDescriptor)(_MIN_ PDyActorCreationDescriptor* mPtrDesc) noexcept;
  void MDY_PRIVATE(DetachDescriptor)(PDyActorCreationDescriptor*) noexcept;

  void MDY_PRIVATE(BindDescriptor)(_MIN_ FDyActor* mPtrDesc) noexcept;
  void MDY_PRIVATE(DetachDescriptor)(FDyActor*) noexcept;

private:
  FDyActor* mPtrActor = nullptr;
  PDyActorCreationDescriptor* mPtrCreationDesc = nullptr;
};

} /// ::dy namespace

#endif /// GUARD_DY_ELEMENT_TYPE_DDYACTORBINDER_H