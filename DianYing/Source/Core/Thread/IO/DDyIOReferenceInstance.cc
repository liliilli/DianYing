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
#include <Dy/Core/Resource/Type/FDyBinderBase.h>

namespace dy
{

void DDyIOReferenceInstance::AttachBinder(_MIN_ const __FDyBinderBase* iPtrBase) noexcept
{
  this->mPtrBoundBinderList.emplace_back(iPtrBase);
}

void DDyIOReferenceInstance::DetachBinder(const __FDyBinderBase* iPtrBase) noexcept
{
  MDY_ASSERT(this->mPtrBoundBinderList.empty() == false, "Reference count must be positive value when detach any binder.");

  const auto itPtr = std::find(MDY_BIND_BEGIN_END(this->mPtrBoundBinderList), iPtrBase);
  MDY_ASSERT(itPtr != this->mPtrBoundBinderList.end(), "Given binder pointer address must be exist in given RI list.");

  DyFastErase(this->mPtrBoundBinderList, std::distance(this->mPtrBoundBinderList.begin(), itPtr));

  if (this->mScope == EDyScope::Temporal && this->mPtrBoundBinderList.empty() == true)
  {
    MDY_NOT_IMPLEMENTED_ASSERT();
  }
}

} /// ::dy namespace