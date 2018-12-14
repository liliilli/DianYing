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
#include <Dy/Core/Resource/Resource/FDyMaterialResource.h>
#include <Dy/Core/Resource/Information/FDyMaterialInformation.h>
#include <Dy/Core/Resource/Information/FDyShaderInformation.h>
#include <Dy/Core/Resource/Information/FDyTextureInformation.h>
#include <Dy/Management/IO/MDyIOResource.h>

namespace dy
{

FDyMaterialResource::FDyMaterialResource(_MIN_ const FDyMaterialInformation& information) :
    mSpecifierName{information.GetSpecifierName()},
    mBlendMode{information.GetBlendMode()}
{
  const auto& ioResource = MDyIOResource::GetInstance();

  const auto ptrShaderInfo = information.GetPtrShaderInformation();
  this->mPtrShaderResource = ioResource.GetPtrInformation<EDyResourceType::GLShader>(ptrShaderInfo->GetSpecifierName());

  const auto& ptrTextureInfoList = information.GetPtrTextureInformationList();
  for (const auto& ptrTextureInfo : ptrTextureInfoList)
  {
    this->mPtrTextureRescList.emplace_back(ioResource.GetPtrInformation<EDyResourceType::Texture>(ptrTextureInfo->GetSpecifierName()));
  }
}

FDyMaterialResource::~FDyMaterialResource()
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

} /// ::dy namespace