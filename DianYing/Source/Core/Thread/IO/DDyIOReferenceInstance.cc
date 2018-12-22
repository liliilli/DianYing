#include <precompiled.h>
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

/// Header file
#include <Dy/Core/Thread/IO/DDyIOReferenceInstance.h>
#include <Dy/Helper/System/Idioms.h>

namespace dy
{

void DDyIOReferenceInstance::AttachBinder(_MIN_ const __FDyBinderBase* iPtrBase) noexcept
{
  this->mPtrBoundBinderList.emplace_back(iPtrBase);
  this->mReferenceCount.fetch_add(1);
}

void DDyIOReferenceInstance::DetachBinder(const __FDyBinderBase* iPtrBase) noexcept
{
  MDY_ASSERT(this->mReferenceCount.load() > 0, "Reference count must be positive value when detach any binder.");

  const auto itPtr = std::find(MDY_BIND_BEGIN_END(this->mPtrBoundBinderList), iPtrBase);
  MDY_ASSERT(itPtr != this->mPtrBoundBinderList.end(), "Given binder pointer address must be exist in given RI list.");

  DyFastErase(this->mPtrBoundBinderList, std::distance(this->mPtrBoundBinderList.begin(), itPtr));
  this->mReferenceCount.fetch_sub(1);

  MDY_NOT_IMPLEMENTED_ASSERT();
}

} /// ::dy namespace