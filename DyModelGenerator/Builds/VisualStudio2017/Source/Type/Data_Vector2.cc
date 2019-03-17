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
#include "../../Include/Type/Data_Vector2.h"
#include <nlohmann/json.hpp>

bool operator==(const DDyVector2& lhs, const DDyVector2& rhs) noexcept
{
  return lhs.X == rhs.X
      && lhs.Y == rhs.Y;
}

bool operator!=(const DDyVector2& lhs, const DDyVector2& rhs) noexcept
{
  return !(lhs == rhs);
}

void to_json(nlohmann::json& j, const DDyVector2& p)
{
  j = nlohmann::json
  {
    {"X", p.X},
    {"Y", p.Y}
  };
}

void from_json(const nlohmann::json& j, DDyVector2& p)
{
  p.X = j["X"].get<float>();
  p.Y = j["Y"].get<float>();
}

