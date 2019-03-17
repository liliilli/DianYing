#ifndef GUARD_DY_ELEMENT_ABSTRACT_ACTOR_ADYACTORBINDERCONTAINER_H
#define GUARD_DY_ELEMENT_ABSTRACT_ACTOR_ADYACTORBINDERCONTAINER_H
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

#include <Dy/Element/Type/DActorBinder.h>
#include <Dy/Helper/System/Pointer.h>

namespace dy
{

template <typename TType>
class ADyActorBinderContainer
{
public:
  virtual ~ADyActorBinderContainer()
  {
    while (this->mPtrBinderList.empty() == false)
    { this->mPtrBinderList.back()->__DetachDescriptor(ptrObject); }
  }

  MDY_NODISCARD EDySuccess 
  MDY_PRIVATE(DetachBinderFromList)(_MIN_ DActorBinder& iRefBinder) noexcept
  {
    TU32 id = 0;
    const TU32 size = this->mPtrBinderList.size();
    for (;id < size; ++id)
    {
      if (this->mPtrBinderList[id] == &iRefBinder) { break; }
    }
    if (id == size) { return DY_FAILURE; }

    FaseErase(this->mPtrBinderList, id);
    return DY_SUCCESS;
  }

  MDY_NODISCARD EDySuccess 
  MDY_PRIVATE(AttachBinderFromList)(_MIN_ DActorBinder& iRefBinder) noexcept
  {
    this->mPtrBinderList.emplace_back(&iRefBinder);
    return DY_SUCCESS;
  }

  /// @brief
  void MDY_PRIVATE(SetPtr)(_MIN_ TType& ptrObject) noexcept { this->ptrObject = ptrObject; }

private:
  std::vector<NotNull<DActorBinder*>> mPtrBinderList;
  TType* ptrObject = nullptr;
};

} /// ::dy namespace

#endif /// GUARD_DY_ELEMENT_ABSTRACT_ACTOR_ADYACTORBINDERCONTAINER_H