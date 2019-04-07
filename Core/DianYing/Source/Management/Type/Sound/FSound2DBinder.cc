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
#include <Dy/Management/Type/Sound/FSound2DBinder.h>
#include <Dy/Management/Type/Sound/FInstantSound2D.h>

namespace dy
{

FSound2DBinder::FSound2DBinder(_MIN_ FInstantSound2D& iRefInstantSound2D) : 
    mPtrInstantSound2DInstance{&iRefInstantSound2D}
{ }

FSound2DBinder::FSound2DBinder(_MIN_ const FSound2DBinder& p) :
    mPtrInstantSound2DInstance{p.mPtrInstantSound2DInstance}
{
  p.mIsCopied = true;
}

FSound2DBinder& FSound2DBinder::operator=(_MIN_ const FSound2DBinder& p)
{
  if (this == &p) { return *this; }

  // If this instance is not copied to something, and not moved to something,
  // Stop and let internal instance release.
  if (this->mIsCopied == false && this->mPtrInstantSound2DInstance != nullptr)
  {
    this->mPtrInstantSound2DInstance->TryStop();
    this->mPtrInstantSound2DInstance = nullptr;
  }

  // Copy
  this->mPtrInstantSound2DInstance = p.mPtrInstantSound2DInstance;
  p.mIsCopied = true;
  return *this;
}

FSound2DBinder::FSound2DBinder(_MIN_ FSound2DBinder&& p) noexcept :
    mPtrInstantSound2DInstance{p.mPtrInstantSound2DInstance}
{
  p.mPtrInstantSound2DInstance = nullptr;
  p.mIsCopied = false;
}

FSound2DBinder& FSound2DBinder::operator==(_MIN_ FSound2DBinder&& p) noexcept
{
  if (this == &p) { return *this; }

  // If this instance is not copied to something, and not moved to something,
  // Stop and let internal instance release.
  if (this->mIsCopied == false && this->mPtrInstantSound2DInstance != nullptr)
  {
    this->mPtrInstantSound2DInstance->TryStop();
    this->mPtrInstantSound2DInstance = nullptr;
  }

  // Copy
  this->mPtrInstantSound2DInstance = p.mPtrInstantSound2DInstance;
  p.mPtrInstantSound2DInstance = nullptr;
  p.mIsCopied = false;
  return *this; 
}

FSound2DBinder::~FSound2DBinder()
{
  // If this instance is not copied to something, and not moved to something,
  // Stop and let internal instance release.
  if (this->mIsCopied == false && this->mPtrInstantSound2DInstance != nullptr)
  {
    this->mPtrInstantSound2DInstance->TryStop();
    this->mPtrInstantSound2DInstance = nullptr;
  }
}

} /// ::dy namespace