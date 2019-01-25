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
#include <Dy/Core/Resource/Information/FDyModelInformation.h>
#include <Dy/Meta/Information/ModelMetaInformation.h>
#include <Dy/Core/Resource/Internal/ModelType.h>
#include <Dy/Helper/System/Idioms.h>

namespace dy
{

FDyModelInformation::FDyModelInformation(_MIN_ const PDyModelInstanceMetaInfo& metaInfo) :
    mSpecifierName{metaInfo.mSpecifierName},
    mModelTransform{metaInfo.mTransform}
{
  for (const auto& [meshSpecifier, materialSpecifier] : metaInfo.mMeshList)
  {
    DySafeUniquePtrEmplaceBack(this->mMeshInformations, meshSpecifier);
    DySafeUniquePtrEmplaceBack(this->mMaterialInformations, materialSpecifier);
  }
}

} /// ::dy namespace