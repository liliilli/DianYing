#ifndef GUARD_DY_MANAGEMENT_SCRIPTMANAGER_H
#define GUARD_DY_MANAGEMENT_SCRIPTMANAGER_H
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

#include <Dy/Management/Interface/ISingletonCrtp.h>

#define SOL_CHECK_ARGUMENT 1
#include <sol2/sol.hpp>

namespace dy
{

///
/// @class MDyScript
/// @brief
///
class MDyScript final : public ISingleton<MDyScript>
{
  MDY_SINGLETON_DERIVED(MDyScript);
  MDY_SINGLETON_PROPERTIES(MDyScript);
public:
  ///
  /// @brief  Get reference of lua instance.
  /// @return lua instance l-value reference.
  ///
  FORCEINLINE MDY_NODISCARD sol::state& GetLuaInstance() noexcept
  {
    return this->mLua;
  }

private:
  sol::state mLua;
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_SCRIPTMANAGER_H