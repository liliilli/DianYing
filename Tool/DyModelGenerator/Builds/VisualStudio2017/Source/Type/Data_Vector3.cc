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
#include "../../Include/Type/Data_Vector3.h"
#include <nlohmann/json.hpp>

bool operator==(const DDyVector3& lhs, const DDyVector3& rhs) noexcept
{
  return lhs.X == rhs.X
      && lhs.Y == rhs.Y;
}

bool operator!=(const DDyVector3& lhs, const DDyVector3& rhs) noexcept
{
  return !(lhs == rhs);
}

void to_json(nlohmann::json& j, const DDyVector3& p)
{
  j = nlohmann::json
  {
    {"X", p.X}, {"Y", p.Y}, {"Z", p.Z}
  };
}

void from_json(const nlohmann::json& j, DDyVector3& p)
{
  p.X = j["X"].get<float>();
  p.Y = j["Y"].get<float>();
  p.Z = j["Z"].get<float>();
}

