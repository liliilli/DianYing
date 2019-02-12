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
#include <Dy/Core/Resource/Resource/FDyTextureResource.h>
#include <Dy/Core/Resource/Information/FDyTextureInformation.h>
#include <Dy/Core/Rendering/Wrapper/PDyGLTextureDescriptor.h>
#include <Dy/Core/Rendering/Wrapper/FDyGLWrapper.h>
#include <Dy/Management/Helper/SDyProfilingHelper.h>
#include "Dy/Core/Resource/Information/FDyTextureGeneralInformation.h"

namespace dy
{

FDyTextureResource::FDyTextureResource(_MIN_ const FDyTextureInformation& information) :
    mInformationBinder{information.GetSpecifierName()}
{
  // Forward dataBuffer's retrieved information to data members.
  this->mTextureName   = information.GetSpecifierName();
  this->mTextureType   = information.GetType();
}

FDyTextureResource::~FDyTextureResource() = default;

const std::string& FDyTextureResource::GetSpecifierName() const noexcept
{
  return this->mTextureName;
}

TU32 FDyTextureResource::GetTextureId() const noexcept
{
  return this->mTextureResourceId;
}

EDyTextureStyleType FDyTextureResource::GetTextureType() const noexcept
{
  return this->mTextureType;
}

} /// ::dy namespace