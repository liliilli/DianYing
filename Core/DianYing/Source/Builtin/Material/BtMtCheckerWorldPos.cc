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
#include <Dy/Builtin/Material/BtMtCheckerWorldPos.h>

#include <Dy/Builtin/Texture/Checker.h>
#include <Dy/Builtin/ShaderGl/Render/BtShUvWorldPos.h>
#include <Dy/Core/Resource/Internal/EMaterialBlendMode.h>

//!
//! Implementation
//!

namespace dy::builtin
{

void FDyBtMtCheckerWorldPos::ConstructBuffer(PDyMaterialInstanceMetaInfo& property) noexcept
{
  property.mSpecifierName   = FDyBtMtCheckerWorldPos::sName;
  property.mShaderSpecifier = FDyBtShGlUvWorldPos::sName;
  property.mBlendMode       = EMaterialBlendMode::Opaque;

  property.mTextureNames[0] = {(FDyBuiltinTextureChecker::sName), ETextureMapType::Unknown};
  PDyMaterialInstanceMetaInfo::InsertValue<EUniformVariableType::Float>(property, "uRoughness", 0.35f);
}

} /// ::dy::builtin namespac