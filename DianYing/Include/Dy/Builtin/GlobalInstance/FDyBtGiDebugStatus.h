#ifndef GUARD_DY_BUILTIN_GlobalInstance_FDyBtGiDebugStatus_H
#define GUARD_DY_BUILTIN_GlobalInstance_FDyBtGiDebugStatus_H
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

#include <Dy/Builtin/Abstract/ADyGlobalInstance.h>
#include <Dy/Core/Reflection/RDyGlobalInstanceManager.h>
#include <Dy/Helper/Type/DCompUpdator.h>

namespace dy
{

MDY_GLOBALINSTANCE_REGISTER(FDyBtGiDebugStatus)
class FDyBtGiDebugStatus final : public ADyGlobalInstance
{
public:
  MDY_GLOBALINSTANCE_PROPERTY(FDyBtGiDebugStatus)

protected:
  static void OnCallbackLogFlag(_MIN_ const bool& iValue);

public:
  DCompUpdator<bool> mLogFlag { false, std::bind(&FDyBtGiDebugStatus::OnCallbackLogFlag, std::placeholders::_1) };
  bool mEditMode = false;
};

}

#endif /// GUARD_DY_BUILTIN_GlobalInstance_FDyBtGiDebugStatus_H