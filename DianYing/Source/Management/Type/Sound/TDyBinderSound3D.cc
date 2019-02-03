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
#include <Dy/Management/Type/Sound/TDyBinderSound3D.h>
#include <Dy/Management/Type/Sound/FDyInstantSound3D.h>

namespace dy
{

TDyBinderSound3D::TDyBinderSound3D(_MIN_ FDyInstantSound3D& iRefInstantSound3D) : 
    mPtrInstantSound3DInstance{&iRefInstantSound3D}
{ }

TDyBinderSound3D::TDyBinderSound3D(_MIN_ const TDyBinderSound3D& p) :
    mPtrInstantSound3DInstance{p.mPtrInstantSound3DInstance}
{
  p.mIsCopied = true;
}

TDyBinderSound3D& TDyBinderSound3D::operator=(_MIN_ const TDyBinderSound3D& p)
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

TDyBinderSound3D::TDyBinderSound3D(_MIN_ TDyBinderSound3D&& p) noexcept :
    mPtrInstantSound3DInstance{p.mPtrInstantSound3DInstance}
{
  p.mPtrInstantSound3DInstance = nullptr;
  p.mIsCopied = false;
}

TDyBinderSound3D& TDyBinderSound3D::operator==(_MIN_ TDyBinderSound3D&& p) noexcept
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

TDyBinderSound3D::~TDyBinderSound3D()
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