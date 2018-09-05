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
#include <Dy/Core/Component/Information/MaterialInformation.h>
#include <Dy/Core/Component/Resource/MaterialResource.h>
#include <Dy/Management/LoggingManager.h>

namespace
{

MDY_SET_IMMUTABLE_STRING(kMaterialInformationTemplate,     "{} | Material information {} : {}");
MDY_SET_IMMUTABLE_STRING(kMaterialInformation,             "DDyMaterialInformation");

} /// ::unnamed namespace

namespace dy
{

DDyMaterialInformation::~DDyMaterialInformation()
{
  MDY_LOG_INFO_D(kMaterialInformationTemplate, kMaterialInformation, "name", this->mMaterialInformation.mMaterialName);
  if (mNextLevelPtr)
  {
#ifdef false
    mNextLevelPtr->__pfSetPrevLevel(nullptr);
#endif
  }
}

std::unique_ptr<DDyMaterialInformation>
DDyMaterialInformation::__pfPopulateWith(const PDyMaterialPopulateDescriptor& desc) const noexcept
{
  MDY_LOG_INFO_D("Populate derived material information {} from {}.",
      desc.mMaterialOverrideName,
      this->mMaterialInformation.mMaterialName);

  auto newDesc = this->mMaterialInformation;
  {
    newDesc.mMaterialName  = desc.mMaterialOverrideName;
    newDesc.mShaderName    = desc.mOverrideShaderName;
  }

  return std::make_unique<std::decay_t<decltype(*this)>>(newDesc);
}

} /// ::dy namespace