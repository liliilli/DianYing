#include <precompiled.h>
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

/// Header file
#include <Dy/Management/Type/Sound/FSound3DBinder.h>
#include <Dy/Management/Type/Sound/FInstantSound3D.h>

namespace dy
{

FSound3DBinder::FSound3DBinder(_MIN_ FInstantSound3D& iRefInstantSound3D) : 
    mPtrInstantSound3DInstance{&iRefInstantSound3D}
{ }

FSound3DBinder::FSound3DBinder(_MIN_ const FSound3DBinder& p) :
    mPtrInstantSound3DInstance{p.mPtrInstantSound3DInstance}
{
  p.mIsCopied = true;
}

FSound3DBinder& FSound3DBinder::operator=(_MIN_ const FSound3DBinder& p)
{
  if (this == &p) { return *this; }

  // If this instance is not copied to something, and not moved to something,
  // Stop and let internal instance release.
  if (this->mIsCopied == false && this->mPtrInstantSound3DInstance != nullptr)
  {
    this->mPtrInstantSound3DInstance->TryStop();
    this->mPtrInstantSound3DInstance = nullptr;
  }

  // Copy
  this->mPtrInstantSound3DInstance = p.mPtrInstantSound3DInstance;
  p.mIsCopied = true;
  return *this;
}

FSound3DBinder::FSound3DBinder(_MIN_ FSound3DBinder&& p) noexcept :
    mPtrInstantSound3DInstance{p.mPtrInstantSound3DInstance}
{
  p.mPtrInstantSound3DInstance = nullptr;
  p.mIsCopied = false;
}

FSound3DBinder& FSound3DBinder::operator==(_MIN_ FSound3DBinder&& p) noexcept
{
  if (this == &p) { return *this; }

  // If this instance is not copied to something, and not moved to something,
  // Stop and let internal instance release.
  if (this->mIsCopied == false && this->mPtrInstantSound3DInstance != nullptr)
  {
    this->mPtrInstantSound3DInstance->TryStop();
    this->mPtrInstantSound3DInstance = nullptr;
  }

  // Copy
  this->mPtrInstantSound3DInstance = p.mPtrInstantSound3DInstance;
  p.mPtrInstantSound3DInstance = nullptr;
  p.mIsCopied = false;
  return *this; 
}

FSound3DBinder::~FSound3DBinder()
{
  // If this instance is not copied to something, and not moved to something,
  // Stop and let internal instance release.
  if (this->mIsCopied == false && this->mPtrInstantSound3DInstance != nullptr)
  {
    this->mPtrInstantSound3DInstance->TryStop();
    this->mPtrInstantSound3DInstance = nullptr;
  }
}

} /// ::dy namespace