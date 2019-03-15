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
#include <Dy/Core/Resource/Resource/FDyAttachmentResource.h>
#include <Dy/Core/Resource/Information/FDyAttachmentInformation.h>
#include <Dy/Core/Rendering/Wrapper/XGLWrapper.h>

namespace dy
{

const std::string& FDyAttachmentResource::GetSpecifierName() const noexcept
{
  return this->mSpecifierName;
}

const EDyGlBufferDataInternalFormat& FDyAttachmentResource::GetBufferType() const noexcept
{
  MDY_ASSERT_MSG(
    this->mInformationBinder.IsResourceExist() == true, 
    "Unexpected error occurred. Information must be valid.");
  return this->mInformationBinder->GetBufferType();
}

const EDyTextureStyleType& FDyAttachmentResource::GetAttachmentType() const noexcept
{
  MDY_ASSERT_MSG(
    this->mInformationBinder.IsResourceExist() == true, 
    "Unexpected error occurred. Information must be valid.");
  return this->mInformationBinder->GetAttachmentType();
}

TU32 FDyAttachmentResource::GetMipmapLevel() const noexcept
{
  MDY_ASSERT_MSG(
    this->mInformationBinder.IsResourceExist() == true, 
    "Unexpected error occurred. Information must be valid.");
  return this->mInformationBinder->GetMipmapLevel();
}

TU32 FDyAttachmentResource::GetDepthNumber() const noexcept
{
  MDY_ASSERT_MSG(
    this->mInformationBinder.IsResourceExist() == true, 
    "Unexpected error occurred. Information must be valid.");
  return this->mInformationBinder->GetDepthNumber();
}

bool FDyAttachmentResource::IsPingPong() const noexcept
{
  return this->mIsPingpong;
}

} /// ::dy namespace