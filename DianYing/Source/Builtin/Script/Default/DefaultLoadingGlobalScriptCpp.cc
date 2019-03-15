#include <precompiled.h>
///
/// MIT License
/// Copyright (c) 2018-2019 Jongmin Yun
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.
///

/// Header file
#include <Dy/Builtin/Script/Default/DefaultLoadingGlobalScriptCpp.h>
#include <Dy/Builtin/Material/OpaqueStaticPlain.h>
#include <Dy/Builtin/Material/BtMtCheckerWorldPos.h>
#include <Dy/Builtin/Material/Transparent/StaticTransparentChecker.h>
#include <Dy/Builtin/Texture/SSAONoiseMap.h>

namespace dy
{

MDY_PRIVATE(FDyDefaultLoadingGlobalScript)::MDY_PRIVATE(FDyDefaultLoadingGlobalScript)()
{
  MDY_LOADING_RESOURCE_BIND(EResourceType::Material, builtin::FDyBuiltinMaterialOpaqueStaticPlain::sName);
  MDY_LOADING_RESOURCE_BIND(EResourceType::Material, builtin::FDyBtMtCheckerWorldPos::sName);
  MDY_LOADING_RESOURCE_BIND(EResourceType::Material, builtin::FDyBtStaticTransparentChecker::sName);

  MDY_LOADING_RESOURCE_BIND(EResourceType::Material, "M_Wall1");
  MDY_LOADING_RESOURCE_BIND(EResourceType::Material, "M_TestDynamicAnimation");

  MDY_LOADING_RESOURCE_BIND(EResourceType::Sound, "TestEffect_C");
}

} /// ::dy namespace