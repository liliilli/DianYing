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
#include <Dy/Core/Component/Resource/ModelResource.h>

#include <Dy/Core/Component/Information/ModelInformation.h>
#include <Dy/Core/Component/Resource/SubmeshResource.h>
#include <Dy/Management/LoggingManager.h>

namespace dy
{

CDyModelResource::~CDyModelResource()
{
  // Release all resource bind to this instance.
  if (this->__mLinkedModelInformationPtr)
  {
    this->__mLinkedModelInformationPtr->__pfSetModelResourceLink(nullptr);
  }

  this->mMeshResource.clear();
}

const std::vector<std::unique_ptr<CDySubmeshResource>>& CDyModelResource::GetSubmeshResources() const noexcept
{
  return this->mMeshResource;
}

EDySuccess CDyModelResource::pInitializeModelResource(const DDyModelInformation& modelInformation)
{
  const auto& submeshInformations = modelInformation.mMeshInformations;
  for (const auto& submeshInformation : submeshInformations)
  {
    std::unique_ptr<CDySubmeshResource> meshResource = std::make_unique<CDySubmeshResource>();
    if (meshResource->pfInitializeSubmeshResource(submeshInformation) == DY_FAILURE)
    {
      MDY_LOG_ERROR("{} | Failed to create submesh resource. | Model name : {}",
                    "CDyModelResource::pInitializeModelResource", modelInformation.mModelName);
      return DY_FAILURE;
    }

    this->mMeshResource.emplace_back(std::move(meshResource));
  }
  return DY_SUCCESS;
}

} /// ::dy namespace