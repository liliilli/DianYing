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

#include <Dy/Helper/Type/DVector2.h>
#include <nlohmann/json_fwd.hpp>

namespace dy
{

/// @struct DArea2D
/// @brief  Area 2D (x, y) (LEFT_DOWN) to (x`, y`) (RIGHT_UP) type.
struct DArea2D final
{
  DVector2 mLeftDown = {};
  DVector2 mRightUp  = {};

  DArea2D() = default;
  DArea2D(_MIN_ const DVector2& iLeftDown, _MIN_ const DVector2& iRightUp) :
      mLeftDown{iLeftDown},
      mRightUp{iRightUp} {};
  DArea2D(_MIN_ TI32 x, _MIN_ TI32 y, _MIN_ TI32 width, _MIN_ TI32 height) :
      DArea2D{
          DVector2{static_cast<TF32>(x), static_cast<TF32>(y)},
          DVector2{static_cast<TF32>(x + width), static_cast<TF32>(y + height)}} {};
};

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const DArea2D& p);
void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DArea2D& p);

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_TYPE_AREA_H