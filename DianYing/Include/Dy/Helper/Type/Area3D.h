#ifndef GAURD_DY_HELPER_TYPE_Area3D_H
#define GAURD_DY_HELPER_TYPE_Area3D_H
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

#include <foundation/PxBounds3.h>
#include <Dy/Helper/Type/Vector3.h>
#include <nlohmann/json_fwd.hpp>

namespace dy
{

///
/// @struct DDyArea3D
/// @brief  Area 3D (x, y, z) (LEFT_DOWN) to (x`, y`, z`) (RIGHT_UP) type.
///
struct DDyArea3D final
{
  DDyVector3 mMin = {};
  DDyVector3 mMax = {};

  DDyArea3D() = default;
  DDyArea3D& operator=(const DDyArea3D&) = default;

  DDyArea3D(_MIN_ const physx::PxBounds3& iBound) : mMin{iBound.minimum}, mMax{iBound.maximum} {};
  DDyArea3D& operator=(_MIN_ const physx::PxBounds3& iBound)
  {
    this->mMin = iBound.minimum;
    this->mMax = iBound.maximum;
    return *this;
  }
};

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const DDyArea3D& p);
void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DDyArea3D& p);

} /// ::dy namespace

#endif /// GAURD_DY_HELPER_TYPE_Area3D_H