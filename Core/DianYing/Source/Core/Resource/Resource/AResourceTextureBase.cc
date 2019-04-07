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
#include <Dy/Core/Resource/Resource/AResourceTextureBase.h>
#include <Dy/Core/Resource/Information/FInformationTexture.h>
#include <Dy/Core/Rendering/Wrapper/PDyGLTextureDescriptor.h>
#include <Dy/Core/Rendering/Wrapper/XGLWrapper.h>

namespace dy
{

AResourceTextureBase::AResourceTextureBase(_MIN_ const FInformationTexture& information) :
    mInformationBinder{information.GetSpecifierName()}
{
  // Forward dataBuffer's retrieved information to data members.
  this->mTextureName   = information.GetSpecifierName();
  this->mTextureType   = information.GetType();
}

AResourceTextureBase::~AResourceTextureBase() = default;

const std::string& AResourceTextureBase::GetSpecifierName() const noexcept
{
  return this->mTextureName;
}

TU32 AResourceTextureBase::GetTextureId() const noexcept
{
  return this->mTextureResourceId;
}

ETextureStyleType AResourceTextureBase::GetTextureType() const noexcept
{
  return this->mTextureType;
}

} /// ::dy namespace