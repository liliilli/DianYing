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

#include <Dy/Core/Resource/Internal/EMaterialBlendMode.h>
#include <nlohmann/json.hpp>
#include <Dy/Helper/System/Assertion.h>

//!
//! Implementation
//!

namespace dy
{

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const EMaterialBlendMode& p)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ EMaterialBlendMode& p)
{
  const auto enumString = j.get<std::string>();
  if      (j == "Opaque")       { p = EMaterialBlendMode::Opaque; }
  else if (j == "Translucent")  { p = EMaterialBlendMode::TranslucentOIT; }
  else if (j == "Custom")       { p = EMaterialBlendMode::Custom; }
  else                          { MDY_UNEXPECTED_BRANCH(); }
}

} /// ::dy namespace