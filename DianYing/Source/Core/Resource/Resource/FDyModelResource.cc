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
#include <Dy/Core/Resource/Information/FDyMaterialInformation.h>
#include <Dy/Helper/System/Idioms.h>

namespace dy
{

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
  for (const auto& materialInformation : input.GetMaterialInformationList())
  {
    MDY_ASSERT(MDY_CHECK_ISNOTEMPTY(materialInformation), "Unexpected error occurred.");
    MDY_ASSERT(materialInformation->IsResourceExist() == true, "Unexpected error occurred.");

    DySafeUniquePtrEmplaceBack(this->mMaterialResource, materialInformation->Get()->GetSpecifierName());
    MDY_ASSERT(this->mMaterialResource.back()->IsResourceExist() == true, "Unexpected error occurred.");
  }
}

} /// ::dy namespace