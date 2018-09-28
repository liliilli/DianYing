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
#include <Dy/Element/Actor.h>

namespace dy
{

void FDyActor::SetParent(NotNull<FDyActor*> validParentRawPtr) noexcept
{
  mParentFDyActorRawPtr = validParentRawPtr;
  MDY_LOG_WARNING("NOT IMPLEMENTED {}", "FDyActor::SetParent");
}

void FDyActor::SetParentRelocateTransform(NotNull<FDyActor*> validParentRawPtr) noexcept
{
  this->SetParent(validParentRawPtr);
  MDY_LOG_WARNING("NOT IMPLEMENTED {}", "FDyActor::SetParentRelocateTransform");
}

void FDyActor::SetParentToRoot() noexcept
{

  MDY_LOG_WARNING("NOT IMPLEMENTED {}", "FDyActor::SetParentToRoot");
}

void FDyActor::SetParentToRootRelocateTransform() noexcept
{
  this->SetParentToRoot();
  MDY_LOG_WARNING("NOT IMPLEMENTED {}", "FDyActor::SetParentToRootRelocateTransform");
}

} /// ::dy namespace