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
/// @TODO IMPLEMEMNT PARAMETER (MIN_ MAG_ MIPMAP_) CUSTOM OPTION MECHANISM.
///

/// Header file
#include <Dy/Core/Component/Information/TextureInformation.h>
#include <Dy/Core/Component/Resource/TextureResource.h>
#include <Dy/Management/LoggingManager.h>

namespace
{

MDY_SET_IMMUTABLE_STRING(kTextureInformationTemplate,   "{} | Texture information {} : {}");
MDY_SET_IMMUTABLE_STRING(kTextureInformationBindTo,     "{} | Texture information binds to {}");

MDY_SET_IMMUTABLE_STRING(kTextureInformation,           "DDyTextureInformation");

} /// ::unnamed namespace

namespace dy
{

DDyTextureInformation::DDyTextureInformation(const PDyTextureConstructionDescriptor& textureConstructionDescriptor) :
    mTextureInformation{textureConstructionDescriptor}
{
  // Set properties and output log for properties.
  MDY_LOG_INFO_D(kTextureInformationTemplate, kTextureInformation, "name", this->mTextureInformation.mTextureName);
  MDY_LOG_INFO_D(kTextureInformationTemplate, kTextureInformation, "local path", this->mTextureInformation.mTextureFileLocalPath);
  MDY_LOG_INFO_D(kTextureInformationTemplate, kTextureInformation, "absolute path", this->mTextureInformation.mTextureFileAbsolutePath);

  switch (this->mTextureInformation.mTextureType)
  {
  case EDyTextureStyleType::D1:
    MDY_LOG_INFO_D(kTextureInformationTemplate, kTextureInformation, "texture type", "Texture1D");
    break;
  case EDyTextureStyleType::D2:
    MDY_LOG_INFO_D(kTextureInformationTemplate, kTextureInformation, "texture type", "Texture2D");
    break;
  default: MDY_UNEXPECTED_BRANCH(); break;
  }

  MDY_LOG_INFO_D(kTextureInformationTemplate, kTextureInformation, "absolute path", this->mTextureInformation.mIsEnabledAbsolutePath ? "ON" : "OFF");
  MDY_LOG_INFO_D(kTextureInformationTemplate, kTextureInformation, "mipmap creation flag", this->mTextureInformation.mIsEnabledCreateMipmap ? "ON" : "OFF");
  MDY_LOG_INFO_D(kTextureInformationTemplate, kTextureInformation, "custom parameter options", this->mTextureInformation.mIsEnabledCustomedTextureParameter ? "ON" : "OFF");

  // @todo output log of map type and parameter options.
}

DDyTextureInformation::~DDyTextureInformation()
{
  MDY_LOG_INFO_D(kTextureInformationTemplate, "~CDyShaderInformation", "name", this->mTextureInformation.mTextureName);
  if (this->__mLinkedTextureResourcePtr) { this->__mLinkedTextureResourcePtr->__pfResetTextureInformationLink(); }
}

void DDyTextureInformation::__pfLinkTextureResource(CDyTextureResource* ptr) const noexcept
{
  MDY_LOG_DEBUG_D(kTextureInformationBindTo, "__pfLinkTextureResource", reinterpret_cast<std::ptrdiff_t>(ptr));
  this->__mLinkedTextureResourcePtr = ptr;
}
} /// ::dy namespace