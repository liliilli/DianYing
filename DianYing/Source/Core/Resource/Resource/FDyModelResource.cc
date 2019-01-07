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
#include <Dy/Core/Resource/Resource/FDyModelResource.h>
#include <Dy/Core/Resource/Information/FDyModelInformation.h>
#include <Dy/Core/Resource/Information/FDyMeshInformation.h>
#include <Dy/Helper/System/Idioms.h>

namespace dy
{

#ifdef false
FDyModelResource::FDyModelResource(_MINOUT_ FDyModelVBOIntermediate& input) :
    mSpecifierName{input.GetSpecifierName()},
    mBinderInformation{input.GetSpecifierName()}
{
  // Intermediate mesh resource list must be mutable, \n
  // so to be transfer properties into actual mesh resource by resetting intermediate instance properties.
  auto& intermediateMeshList = input.GetIntermediateMeshList();
  this->mMeshResource.reserve(intermediateMeshList.size());

  for (auto& intermediateMesh : intermediateMeshList) { mMeshResource.emplace_back(intermediateMesh); }
}
#endif

FDyModelResource::FDyModelResource(_MINOUT_ const FDyModelInformation& input) :
  mSpecifierName{input.GetSpecifierName()},
  mBinderInformation{input.GetSpecifierName()}
{
  for (const auto& meshInformation : input.GetMeshInformationList())
  {
    MDY_ASSERT(MDY_CHECK_ISNOTEMPTY(meshInformation), "Unexpected error occurred.");
    MDY_ASSERT(meshInformation->IsResourceExist() == true, "Unexpected error occurred.");

    DySafeUniquePtrEmplaceBack(this->mMeshResource, meshInformation->Get()->GetSpecifierName());
    MDY_ASSERT(this->mMeshResource.back()->IsResourceExist() == true, "Unexpected error occurred.");
  }
}

} /// ::dy namespace