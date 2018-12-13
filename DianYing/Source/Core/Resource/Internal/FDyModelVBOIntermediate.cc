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
#include <Dy/Core/Resource/Internal/FDyModelVBOIntermediate.h>
#include <Dy/Core/Resource/Information/FDyModelInformation.h>

namespace dy
{

FDyModelVBOIntermediate::FDyModelVBOIntermediate(_MIN_ const FDyModelInformation& information)
{
  this->mSpecifierName = information.GetSpecifierName();

  const auto& submeshInformations = information.GetMeshInformationList();
  for (const auto& submeshInformation : submeshInformations)
  {
    this->mIntermediateMeshResourceList.emplace_back(submeshInformation);

    // @TODO IMPLEMENT SKELETAL ANIMATION.
#ifdef false
    if (submeshInformation.GetInformation().mIsEnabledSkeletalAnimation)
    {
      this->mIsEnabledModelSkeletalAnimation = true;
      MDY_LOG_DEBUG_D("{} | Model has bone so can be animated. | Model name : {}", "CDyModelResource::pInitializeModelResource", modelInformation.mModelName);
    }
#endif
  }
}

} /// ::dy namespace