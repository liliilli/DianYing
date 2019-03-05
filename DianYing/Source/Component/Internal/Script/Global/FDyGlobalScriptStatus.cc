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
#include <Dy/Component/Internal/Script/FDyGlobalScriptStatus.h>
#include <Dy/Component/Internal/Global/CDyGlobalScriptCpp.h>

namespace dy
{

FDyGlobalScriptState::FDyGlobalScriptState(_MIN_ const PDyScriptInstanceMetaInfo& descriptor) :
    mType{descriptor.mScriptType}
{
  switch (this->mType)
  {
  case EDyScriptType::Cpp: 
  { // Cpp
    this->mScriptInstance = std::make_unique<CDyGlobalScriptCpp>(descriptor);
  } break;
  case EDyScriptType::Lua: 
  { // Lua
    MDY_NOT_IMPLEMENTED_ASSERT();
  } break;
  default: MDY_NOT_IMPLEMENTED_ASSERT();
  }
}

void FDyGlobalScriptState::CallStart()
{
  this->mScriptInstance->OnStart();
}

void FDyGlobalScriptState::CallEnd()
{
  this->mScriptInstance->OnEnd();
}

EDyScriptType FDyGlobalScriptState::GetScriptType() const noexcept
{
  MDY_ASSERT_MSG(this->mType != decltype(this->mType)::NoneError, "Script type must be specified properly.");
  return this->mType;
}

CDyGlobalScriptBase* FDyGlobalScriptState::__GetPtrInternalGlobalScript() const noexcept
{
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTEMPTY(this->mScriptInstance), "Internal script instance must be valid.");
  return this->mScriptInstance.get();
}

} /// ::dy namespace
