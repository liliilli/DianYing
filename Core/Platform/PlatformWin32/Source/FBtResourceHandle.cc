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

#include <FBtResourceHandle.h>
#include <cassert>

namespace dy
{

FBtResourceHandle::FBtResourceHandle(HRSRC validResource)
  : mValidResource{validResource},
    mResourceHnd{LoadResource(nullptr, this->mValidResource)}
{ }

FBtResourceHandle::~FBtResourceHandle()
{
  // If this instance has valid resource, free resource.
  if (this->mValidResource != nullptr)
  {
    assert(this->mResourceHnd != nullptr);

    FreeResource(this->mResourceHnd);
    this->mResourceHnd = nullptr;
    this->mValidResource = nullptr;
  }
}

FBtResourceHandle::FBtResourceHandle(FBtResourceHandle&& from) noexcept
  : mValidResource{from.mValidResource},
    mResourceHnd{from.mResourceHnd}
{
  from.mValidResource = nullptr;
  from.mResourceHnd = nullptr;
}

FBtResourceHandle& FBtResourceHandle::operator=(FBtResourceHandle&& from) noexcept
{
  // Check itself.
  if (this == &from) { return *this; }

  // Check this resource is valid.
  if (this->mValidResource != nullptr)
  {
    // Check resource pointer is same.
    if (this->mValidResource == from.mValidResource) { return *this; }

    // Free resource of this.
    FreeResource(this->mResourceHnd);
  }
  
  // Move resource pointers of from `from` to `this`.
  this->mValidResource = from.mValidResource;
  this->mResourceHnd = from.mResourceHnd;
  return *this;
}

void* FBtResourceHandle::GetBufferPtr()
{
  if (this->mValidResource == nullptr) { return nullptr; }

  return LockResource(this->mResourceHnd);
}

size_t FBtResourceHandle::GetResourceSize()
{
  if (this->mValidResource == nullptr) { return 0; }

  return SizeofResource(nullptr, this->mValidResource);
}

} /// ::dy namespace