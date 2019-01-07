#ifndef GUARD_DY_HELPER_TYPE_AREA_H
#define GUARD_DY_HELPER_TYPE_AREA_H
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

#include <Dy/Helper/Type/Vector2.h>
#include <nlohmann/json_fwd.hpp>

namespace dy
{

///
/// @struct DDyArea2D
/// @brief  Area 2D (x, y) (LEFT_DOWN) to (x`, y`) (RIGHT_UP) type.
///
struct DDyArea2D final
{
  DDyVector2 mLeftDown = {};
  DDyVector2 mRightUp  = {};
};

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const DDyArea2D& p);
void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DDyArea2D& p);

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_TYPE_AREA_H