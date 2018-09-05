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
#include <Dy/Core/Component/Information/TextureInformation.h>
#include <Dy/Core/Component/Resource/TextureResource.h>
#include <Dy/Management/LoggingManager.h>

namespace
{

MDY_SET_IMMUTABLE_STRING(kTextureInformationTemplate,   "{} | Texture information {} : {}");
MDY_SET_IMMUTABLE_STRING(kTextureInformationTypeError,  "{} | Texture information type must be specified.");
MDY_SET_IMMUTABLE_STRING(kTextureInformationBindTo,     "{} | Texture information binds to {}");
MDY_SET_IMMUTABLE_STRING(kTextureInformation,           "CDyTextureInformation");

} /// ::unnamed namespace

namespace dy
{

CDyTextureInformation::CDyTextureInformation(const PDyTextureConstructionDescriptor& textureConstructionDescriptor) :
    mTextureInformation{textureConstructionDescriptor}
{
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
  default:
    MDY_LOG_CRITICAL_D(kTextureInformationTypeError, kTextureInformation);
    assert(false);
    break;
  }

  MDY_LOG_INFO_D(kTextureInformationTemplate, kTextureInformation, "absolute path", this->mTextureInformation.mIsEnabledAbsolutePath ? "ON" : "OFF");
  MDY_LOG_INFO_D(kTextureInformationTemplate, kTextureInformation, "mipmap creation flag", this->mTextureInformation.mIsEnabledCreateMipmap ? "ON" : "OFF");
  MDY_LOG_INFO_D(kTextureInformationTemplate, kTextureInformation, "custom parameter options", this->mTextureInformation.mIsEnabledCustomedTextureParameter ? "ON" : "OFF");

  // @todo output log of map type and parameter options.
}

CDyTextureInformation::~CDyTextureInformation()
{
  MDY_LOG_INFO_D(kTextureInformationTemplate, "~CDyShaderInformation", "name", this->mTextureInformation.mTextureName);
  if (mNextLevelPtr)
  {
    mNextLevelPtr->__pfSetPrevLevel(nullptr);
  }
}

void CDyTextureInformation::__pfSetNextLevel(CDyTextureResource* ptr) const noexcept
{
  MDY_LOG_DEBUG_D(kTextureInformationBindTo, "__pfSetNextLevel", reinterpret_cast<std::ptrdiff_t>(ptr));
  mNextLevelPtr = ptr;
}
} /// ::dy namespace