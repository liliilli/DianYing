#include <precompiled.h>
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

/// Header file
#include <Dy/Helper/Type/Area2D.h>

#include <nlohmann/json.hpp>
#include <Dy/Helper/Library/HelperJson.h>

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

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const DDyArea2D& p)
{
  j = nlohmann::json
  {
      {MSVSTR(sHeader_LeftDown), p.mLeftDown},
      {MSVSTR(sHeader_RightUp),  p.mRightUp}
  };
}

void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DDyArea2D& p)
{
  p.mLeftDown = DyGetValue<DDyVector2>(j, sHeader_LeftDown);
  p.mRightUp  = DyGetValue<DDyVector2>(j, sHeader_RightUp);
}

} /// ::dy namespace