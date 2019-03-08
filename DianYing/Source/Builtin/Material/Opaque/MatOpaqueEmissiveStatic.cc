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
#include <Dy/Builtin/Material/Opaque/MatOpaqueEmissiveStatic.h>

#include <Dy/Builtin/Texture/Checker.h>
#include <Dy/Core/Resource/Internal/MaterialType.h>
#include <Dy/Builtin/ShaderGl/Render/OpaqueEmissiveStatic.h>

//!
//! Implementation
//!

namespace dy::builtin
{

void FDyBtMtOpaqueEmissiveStatic::ConstructBuffer(PDyMaterialInstanceMetaInfo& oProperty) noexcept
{
  oProperty.mSpecifierName   = sName;
  oProperty.mShaderSpecifier = FDyBtShGlEmissiveStatic::sName;
  oProperty.mBlendMode       = EDyMaterialBlendMode::Opaque;

  oProperty.mTextureNames[0] = {(FDyBuiltinTextureChecker::sName), EDyTextureMapType::Unknown};
  PDyMaterialInstanceMetaInfo::InsertValue<EDyUniformVariableType::Float>(
    oProperty, 
    "uEmissiveFactor", 4.0f);
}

} /// ::dy::builtin namespac