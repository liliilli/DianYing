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
#include <Dy/Helper/Type/DMatrix2x2.h>
#include <Dy/Helper/Type/DVector2.h>
#include <nlohmann/json.hpp>

namespace dy::math
{

void to_json(nlohmann::json& j, const DMat2& p)
{
  j = p.ToVector();
}

void from_json(const nlohmann::json& j, DMat2& p)
{
  const auto vecs = j.get<decltype(p.ToVector())>();
  for (TIndex i = 0; i < 2; ++i)
  {
    p[i] = vecs[i];
  }
}

} /// ::dy::math namespace