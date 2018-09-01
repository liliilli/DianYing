#include <precompiled.h>
///
/// MIT License
/// Copyright (c) 2018 Jongmin Yun
///
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
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

namespace dy
{

DDyMaterialInformation::~DDyMaterialInformation()
{
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
  auto newDesc = this->mMaterialInformation;
  {
    newDesc.mMaterialName  = desc.mMaterialOverrideName;
    newDesc.mShaderName    = desc.mOverrideShaderName;
  }

  auto newMaterial = std::make_unique<std::decay_t<decltype(*this)>>(newDesc);
  return std::move(newMaterial);
}

} /// ::dy namespace