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

#include <Dy/Helper/Type/DBounds.h>
#include <nlohmann/json.hpp>
#include <foundation/PxBounds3.h>
#include <Dy/Helper/Library/HelperJson.h>
#include <Dy/Helper/Type/DVector2.h>
#include <Dy/Helper/Type/DVector3.h>

namespace dy
{

DBounds3D FBounds3D::CreateWith(const physx::PxBounds3& iBound)
{
  return {FVec3::CreateVec3(iBound.minimum), FVec3::CreateVec3(iBound.maximum)};
}

} /// ::dy namespace


namespace dy::math
{

void to_json(nlohmann::json& oJson, const DBounds2D<TReal>& iBounds)
{
  oJson = nlohmann::json
  {
    {"Min", iBounds.GetMinimumPoint()},
    {"Max", iBounds.GetMaximumPoint()}
  };
}

void from_json(const nlohmann::json& iJson, DBounds2D<TReal>& oBounds)
{
  oBounds = DBounds2D<TReal>
  {
    json::GetValueFrom<DBounds2D<TReal>::TItem>(iJson, "Min"),
    json::GetValueFrom<DBounds2D<TReal>::TItem>(iJson, "Max"),
  };
}


void to_json(nlohmann::json& oJson, const DBounds3D<TReal>& iBounds)
{
  oJson = nlohmann::json
  {
    {"Min", iBounds.GetMinimumPoint()},
    {"Max", iBounds.GetMaximumPoint()}
  };;
}

void from_json(const nlohmann::json& iJson, DBounds3D<TReal>& oBounds)
{
  oBounds = DBounds3D<TReal>
  {
    json::GetValueFrom<DBounds3D<TReal>::TItem>(iJson, "Min"),
    json::GetValueFrom<DBounds3D<TReal>::TItem>(iJson, "Max"),
  };;
}

} /// ::dy namespace