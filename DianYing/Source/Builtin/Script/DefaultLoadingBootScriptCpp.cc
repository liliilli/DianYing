#include <precompiled.h>
///
/// MIT License
/// Copyright (c) 2018 Jongmin Yun
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
#include <Dy/Builtin/Script/DefaultLoadingBootScriptCpp.h>
#include <Dy/Builtin/Texture/Checker.h>
#include <Dy/Builtin/Texture/ErrorBlue.h>

#define MDY_LOADING_RESOURCE_BIND(__MAResourceType__, __MAString__) \
    this->mResourceRequisitionList.emplace_back(__MAResourceType__, __MAString__)

namespace dy
{

FDyDefaultLoadingBootScript::FDyDefaultLoadingBootScript()
{
  MDY_LOADING_RESOURCE_BIND(EDyResourceType::Texture, builtin::FDyBuiltinTextureChecker::sName);
  MDY_LOADING_RESOURCE_BIND(EDyResourceType::Texture, builtin::FDyBuiltinTextureErrorBlue::sName);
}

} /// ::dy namespace