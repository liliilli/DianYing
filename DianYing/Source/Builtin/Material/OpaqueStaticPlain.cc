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
#include <Dy/Builtin/Material/OpaqueStaticPlain.h>

#include <Dy/Builtin/Texture/Checker.h>
#include <Dy/Builtin/ShaderGl/RenderOpaqueStatic.h>
#include <Dy/Core/Component/Internal/MaterialType.h>
#include <Dy/Management/DataInformationManager.h>
#include "Dy/Builtin/Texture/ErrorBlue.h"

//!
//! Implementation
//!

namespace dy::builtin
{

FDyBuiltinMaterialOpaqueStaticPlain::FDyBuiltinMaterialOpaqueStaticPlain()
{
  auto& manInfo = MDyDataInformation::GetInstance();

  PDyMaterialConstructionDescriptor matDesc;
  matDesc.mMaterialName = FDyBuiltinMaterialOpaqueStaticPlain::sName;
  matDesc.mShaderName   = FDyBuiltinShaderGLRenderOpaqueStatic::sName;
  matDesc.mBlendMode    = EDyMaterialBlendMode::Opaque;

  matDesc.mTextureNames.emplace_back(MSVSTR(FDyBuiltinTextureErrorBlue::sName));
  MDY_CALL_ASSERT_SUCCESS(manInfo.CreateMaterialInformation(matDesc));
}

} /// ::dy::builtin namespace