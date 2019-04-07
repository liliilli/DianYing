#ifndef GUARD_DY_HELPER_INTERNAL_INLINE_FCALLSTASCKTMP_INL
#define GUARD_DY_HELPER_INTERNAL_INLINE_FCALLSTASCKTMP_INL
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

template <typename TType>
void FCallStack<TType>::SetCallback(TRawCallback iPtrCallback)
{ 
  this->mCallback = iPtrCallback; 
}

template <typename TType>
template <typename TClass>
void FCallStack<TType>::SetCallback(TClass& iRefClass, TMemberCallback<TClass> iPtrMemberCallback)
{
  using namespace std::placeholders;
  this->mCallback = std::bind(iPtrMemberCallback, &iRefClass, _1);
}

template <typename TType>
void FCallStack<TType>::CallCallback()
{
  if (this->IsEmpty() == false && this->mCallback != nullptr)
  {
    this->mCallback(this->mInternalStack.top());
  }
}

template <typename TType>
void FCallStack<TType>::Push(TType&& ioValue, bool iCallCallback)
{
  this->mInternalStack.push(std::move(ioValue));
  if (iCallCallback == true) { this->CallCallback(); }
}

template <typename TType>
void FCallStack<TType>::Push(const TType& iValue, bool iCallCallback)
{
  this->mInternalStack.push(iValue);
  if (iCallCallback == true) { this->CallCallback(); }
}

template <typename TType>
const TType& FCallStack<TType>::Top() const
{
  return this->mInternalStack.top();
}

template <typename TType>
TType FCallStack<TType>::ExtractTop(bool iCallCallback)
{
  //
  TType result = std::move(this->mInternalStack.top());
  this->mInternalStack.pop();
  //
  if (iCallCallback == true) { this->CallCallback(); }
  //
  return std::move(result);
}

template <typename TType>
void FCallStack<TType>::Pop(const bool iCallCallback)
{
  //
  this->mInternalStack.pop(); 
  //
  if (iCallCallback == true) { this->CallCallback(); }
}

template <typename TType>
bool FCallStack<TType>::IsEmpty() const noexcept
{
  return this->mInternalStack.empty(); 
}

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_INTERNAL_INLINE_FCALLSTASCKTMP_INL