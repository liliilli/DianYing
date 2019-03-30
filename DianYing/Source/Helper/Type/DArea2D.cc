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
#include <Dy/Helper/Type/DArea2D.h>

#include <nlohmann/json.hpp>
#include <Dy/Helper/Library/HelperJson.h>
#include <Dy/Helper/Type/DVector2.h>

//!
//! Forward declaration & local translation unit data
//!

namespace
{

MDY_SET_IMMUTABLE_STRING(sHeader_LeftDown,  "LeftDown");
MDY_SET_IMMUTABLE_STRING(sHeader_RightUp,   "RightUp");

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy
{

#ifdef false
void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const DArea2D& p)
{
  j = nlohmann::json
  {
      {(sHeader_LeftDown), p.mLeftDown},
      {(sHeader_RightUp),  p.mRightUp}
  };
}

void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DArea2D& p)
{
  p.mLeftDown = json::GetValueFrom<DVec2>(j, sHeader_LeftDown);
  p.mRightUp  = json::GetValueFrom<DVec2>(j, sHeader_RightUp);
}
#endif

void math::to_json(nlohmann::json& oJson, const DArea2D<TReal>& iArea)
{
  oJson = nlohmann::json
  {
    {"Start", iArea.GetStartPoint()},
    {"Length", iArea.GetWh()}
  };
}

void math::from_json(const nlohmann::json& iJson, DArea2D<TReal>& oArea)
{
  oArea.SetStartPoint(json::GetValueFrom<DVec2>(iJson, "Start"));
  oArea.SetWh(json::GetValueFrom<DVec2>(iJson, "Length"));
}

} /// ::dy namespace