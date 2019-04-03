#ifndef GUARD_DY_HELPER_TYPE_MATRIX3_H
#define GUARD_DY_HELPER_TYPE_MATRIX3_H
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

#include <glm/fwd.hpp>
#include <nlohmann/json_fwd.hpp>
#include <Math/Type/Math/DMatrix3.h>

namespace dy
{

using DMat3 = math::DMatrix3<TReal>;

struct FMat3 final
{
  static DMat3 CreateMat3(const glm::mat3& mat);
};

} /// ::dy namespace

namespace dy::math
{
  
void to_json(nlohmann::json& j, const DMat3& p);
void from_json(const nlohmann::json& j, DMat3& p);

} /// ::dy::math namespace

#endif /// GUARD_DY_HELPER_TYPE_MATRIX3_H