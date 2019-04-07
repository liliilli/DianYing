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
#include <Dy/Builtin/Material/Transparent/StaticTransparentChecker.h>
#include <Dy/Builtin/ShaderGl/Level/BtShOITAccumulation.h>
#include <Dy/Builtin/Texture/CheckerBright.h>

namespace dy::builtin
{

void FDyBtStaticTransparentChecker::ConstructBuffer(PDyMaterialInstanceMetaInfo& property) noexcept
{
  property.mSpecifierName   = sName;
  property.mShaderSpecifier = FDyBtShOITAccumulation::sName;
  property.mBlendMode       = EMaterialBlendMode::TranslucentOIT;

  property.mTextureNames[0] = {(FDyBtTexCheckerBright::sName), ETextureMapType::Unknown};
}

} /// ::dy::builtin namespace