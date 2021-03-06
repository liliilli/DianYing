#ifndef GUARD_DY_COMPONENT_ABSTRACT_AGlobalCppScript_H
#define GUARD_DY_COMPONENT_ABSTRACT_AGlobalCppScript_H
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

#include <Dy/Component/Interface/IScriptable.h>

namespace dy
{

/// @class AGlobalCppScript
/// @brief Global cpp script for initiate when initialization phase & end phase.
class AGlobalCppScript : public IScriptable
{
public:
  virtual ~AGlobalCppScript() = default;
  virtual void OnStart() = 0;
  virtual void OnEnd() = 0;

private:
  void Initiate() override final {};
  void Start() override final {};
  void Update(TF32) override final {};
  void Destroy() override final {};
  void OnEnabled() override final {};
  void OnDisabled() override final {};
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_ABSTRACT_AGlobalCppScript_H