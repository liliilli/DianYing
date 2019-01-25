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

#include <Dy/Component/Internal/Global/CDyGlobalScriptCpp.h>
#include "Dy/Helper/Wrapper/DDyMutexUniqueHashMap.h"

namespace dy
{

CDyGlobalScriptCpp::CDyGlobalScriptCpp(_MIN_ const PDyScriptInstanceMetaInfo& descriptor)
{
  MDY_ASSERT(descriptor.mScriptType == EDyScriptType::Cpp,    "Script type is not matched to CDyWidgetScriptCpp.");
  MDY_ASSERT(descriptor.mScriptMode == EDyScriptMode::Global, "Given script must be global type.");

  MDY_ASSERT(MDY_CHECK_ISNOTNULL(descriptor.mBtInstantiationFunction), "Cpp script instantiation function must be not null.");
  this->mScriptInstance = DyConvertUniquePtrTo<ADyGlobalCppScript>(descriptor.mBtInstantiationFunction());
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(this->mScriptInstance),    "Script instance could not bound to system.");

  this->mScriptName = descriptor.mSpecifierName;
}

ADyGlobalCppScript* CDyGlobalScriptCpp::__GetScriptInstance() const noexcept
{
  MDY_ASSERT(MDY_CHECK_ISNOTEMPTY(this->mScriptInstance), "Script instance must be valid, not empty.");
  return this->mScriptInstance.get();
}

void CDyGlobalScriptCpp::OnStart()
{
  this->mScriptInstance->OnStart();
}

void CDyGlobalScriptCpp::OnEnd()
{
  this->mScriptInstance->OnEnd();
}

} /// ::dy namespace