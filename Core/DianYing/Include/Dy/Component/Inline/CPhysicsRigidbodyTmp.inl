#ifndef GUARD_DY_COMPONENT_INLINE_CPHYSICSRIGIDBDOYTMP_INL
#define GUARD_DY_COMPONENT_INLINE_CPHYSICSRIGIDBDOYTMP_INL
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

namespace dy
{

template <ECollisionCallbackType TCbType, typename TScriptType>
EDySuccess CPhysicsRigidbody::AddCollisionCallback(TScriptType& iRefScript,
  typename TCollisionCbTypeSignature<TCbType>::Type::template RawType<TScriptType> iRawFunction)
{
  static_assert(IsInheritancedFrom<TScriptType, AActorCppScript> == true, "TScriptType must be inheritenced from AActorCppScript.");
  if constexpr (TCbType == ECollisionCallbackType::OnHit)
  {
    // Try insert 
    auto* ptrItem = this->mCallbackContainer.onHit.AddCallback(iRefScript, iRawFunction);
    if (ptrItem == nullptr) { return EDySuccess::DY_FAILURE; } 
    // Try bind item (unique-id specifically) into Script.
    iRefScript.MDY_PRIVATE(BindCollisionCbHandle)(*this, TCbType, (void*&)iRawFunction);
    return EDySuccess::DY_SUCCESS;
  }
  else if constexpr (TCbType == ECollisionCallbackType::OnOverlapBegin) 
  {
    // Try insert 
    auto* ptrItem = this->mCallbackContainer.onOverlapBegin.AddCallback(iRefScript, iRawFunction);
    if (ptrItem == nullptr) { return EDySuccess::DY_FAILURE; } 
    // Try bind item (unique-id specifically) into Script.
    iRefScript.MDY_PRIVATE(BindCollisionCbHandle)(*this, TCbType, (void*&)iRawFunction);
    return EDySuccess::DY_SUCCESS;
  }
  else if constexpr (TCbType == ECollisionCallbackType::OnOverlapEnd)
  {
    // Try insert 
    auto* ptrItem = this->mCallbackContainer.onOverlapEnd.AddCallback(iRefScript, iRawFunction);
    if (ptrItem == nullptr) { return EDySuccess::DY_FAILURE; } 
    // Try bind item (unique-id specifically) into Script.
    iRefScript.MDY_PRIVATE(BindCollisionCbHandle)(*this, TCbType, (void*&)iRawFunction);
    return EDySuccess::DY_SUCCESS;
  }
  else
  {
    MDY_UNEXPECTED_BRANCH();
    return EDySuccess::DY_FAILURE;
  }
}

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_INLINE_CPHYSICSRIGIDBDOYTMP_INL