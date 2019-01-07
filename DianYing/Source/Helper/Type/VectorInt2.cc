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
#include <Dy/Helper/Type/VectorInt2.h>
#include <nlohmann/json.hpp>

namespace dy
{

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const DDyVectorInt2& p)
{
  j = nlohmann::json
  {
      {"X", p.X},
      {"Y", p.Y}
  };
}

void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DDyVectorInt2& p)
{
  p.X = j.at("X").get<TI32>();
  p.Y = j.at("Y").get<TI32>();
}

} /// ::dy namespce