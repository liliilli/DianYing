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
#include <Dy/Core/Component/Information/ShaderInformation.h>
#include <Dy/Core/Component/Resource/ShaderResource.h>
#include <Dy/Management/LoggingManager.h>

namespace
{

MDY_SET_IMMUTABLE_STRING(kShaderInformationTemplate,    "{} | Shader information {} : {}");
MDY_SET_IMMUTABLE_STRING(kShaderFragmentTemplate,       "{} | Shader fragment {} : {}");
MDY_SET_IMMUTABLE_STRING(kShaderInformationBindTo,      "{} | Shader information binds to {}");
MDY_SET_IMMUTABLE_STRING(kShaderInformation,            "CDyShaderInformation");

} /// ::unnamed namespace

namespace dy
{

CDyShaderInformation::CDyShaderInformation(const PDyShaderConstructionDescriptor& shaderConstructionDescriptor) :
    mShaderInformation{shaderConstructionDescriptor} {
  MDY_LOG_INFO_D(kShaderInformationTemplate, kShaderInformation, "name", this->mShaderInformation.mShaderName);

  for (const auto& fragment : this->mShaderInformation.mShaderFragments)
  {
    MDY_LOG_INFO_D(kShaderFragmentTemplate, kShaderInformation, "path", fragment.mShaderPath);
    // @todo and output fragment type also.
  }
}

CDyShaderInformation::~CDyShaderInformation()
{
  MDY_LOG_INFO_D(kShaderInformationTemplate, "~CDyShaderInformation", "name", this->mShaderInformation.mShaderName);

  if (mNextLevelPtr)
  {
    mNextLevelPtr->__pfSetPrevLevel(nullptr);
  }
}

void CDyShaderInformation::__pfSetNextLevel(CDyShaderResource* ptr) const noexcept
{
  MDY_LOG_DEBUG_D(kShaderInformationBindTo, "__pfSetNextLevel", reinterpret_cast<std::ptrdiff_t>(ptr));
  mNextLevelPtr = ptr;
}

} /// ::dy namespace