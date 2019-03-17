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
#include "../../Include/Type/Data_Vector4.h"
#include <nlohmann/json.hpp>

bool operator==( const DDyVector4& lhs,  const DDyVector4& rhs) noexcept
{
  return lhs.X == rhs.X 
      && lhs.Y == rhs.Y
      && lhs.Z == rhs.Z
      && lhs.W == rhs.W;
}

bool operator!=( const DDyVector4& lhs,  const DDyVector4& rhs) noexcept
{
  return !(lhs == rhs);
}

void to_json(nlohmann::json& j, const DDyVector4& p)
{
  j = nlohmann::json
  {
    {"X", static_cast<float>(p.X)}, 
    {"Y", static_cast<float>(p.Y)}, 
    {"Z", static_cast<float>(p.Z)}, 
    {"W", static_cast<float>(p.W)}
  };
}

void from_json(const nlohmann::json& j, DDyVector4& p)
{
  p.X = j["X"].get<float>();
  p.Y = j["Y"].get<float>();
  p.Z = j["Z"].get<float>();
  p.W = j["W"].get<float>();
}
