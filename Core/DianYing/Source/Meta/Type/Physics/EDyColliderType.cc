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
#include <Dy/Meta/Type/Physics/EDyColliderType.h>
#include <Dy/Helper/Internal/XStringSwitch.h>
#include <Dy/Helper/System/Assertion.h>

namespace dy
{

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ EDyColliderType& p)
{
  const auto stringValue = j.get<std::string>();
  
  switch (string::Input(stringValue))
  {
  case string::Case("Sphere"):   p = EDyColliderType::Sphere; break;
  case string::Case("Capsule"):  p = EDyColliderType::Capsule; break;
  case string::Case("Box"):      p = EDyColliderType::Box; break;
  default: MDY_UNEXPECTED_BRANCH(); break;
  }
}

} /// ::dy namespace