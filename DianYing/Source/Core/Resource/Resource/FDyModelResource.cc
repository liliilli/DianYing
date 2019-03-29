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
#include <Dy/Builtin/Constant/GeneralValue.h>
#include <Dy/Helper/System/Idioms.h>

namespace dy
{

FDyModelResource::FDyModelResource(const FDyModelInformation& input) :
  mSpecifierName{input.GetSpecifierName()},
  mBinderInformation{input.GetSpecifierName()},
  mSupportInstancingFlags{input.GetInstancingFlags()}
{
  size_t i = 0;
  for (const auto& meshInformation : input.GetMeshInformationList())
  {
    MDY_ASSERT_MSG(MDY_CHECK_ISNOTEMPTY(meshInformation), "Unexpected error occurred.");
    MDY_ASSERT_MSG(meshInformation->IsResourceExist() == true, "Unexpected error occurred.");

    auto meshName = meshInformation->Get()->GetSpecifierName();
    if (this->mSupportInstancingFlags[i] == true)
    {
      meshName += kInstancingPostfix;
    }

    SafeUniquePtrEmplaceBack(this->mMeshResource, meshName);
    MDY_ASSERT_MSG(this->mMeshResource.back()->IsResourceExist() == true, "Unexpected error occurred.");
    i++;
  }

  i = 0;
  for (const auto& materialInformation : input.GetMaterialInformationList())
  {
    MDY_ASSERT_MSG(MDY_CHECK_ISNOTEMPTY(materialInformation), "Unexpected error occurred.");
    MDY_ASSERT_MSG(materialInformation->IsResourceExist() == true, "Unexpected error occurred.");

    SafeUniquePtrEmplaceBack(this->mMaterialResource, materialInformation->Get()->GetSpecifierName());
    MDY_ASSERT_MSG(this->mMaterialResource.back()->IsResourceExist() == true, "Unexpected error occurred.");
    i++;
  }

  const auto& transform = input.GetInitialTransform();
  this->mModelInitialTransform = FMat4::Translate(
    FMat4::Rotate(
      FMat4::Scale(this->mModelInitialTransform, transform.mScale)
      , transform.mRotation)
    , transform.mPosition);
}

TU32 FDyModelResource::GetNumberOfMaterial() const noexcept
{
  return this->mMaterialResource.size();
}

TDyResourceBinderMaterial* FDyModelResource::GetMaterialBinder(_MIN_ TU32 iIndex) noexcept
{
  // Check bound
  if (iIndex >= this->mMaterialResource.size()) { return nullptr; }

  return this->mMaterialResource[iIndex].get();
}

} /// ::dy namespace