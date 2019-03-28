#ifndef GUARD_DY_HELPER_TYPE_MATRIX2_H
#define GUARD_DY_HELPER_TYPE_MATRIX2_H
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

#include <Math/Type/Math/DMatrix2.h>
#include <nlohmann/json_fwd.hpp>

namespace dy
{

using DMat2 = math::DMatrix2<TReal>;

} /// ::dy namespace

namespace dy::math
{
  
void to_json(nlohmann::json& j, const DMat2& p);
void from_json(const nlohmann::json& j, DMat2& p);

} /// ::dy::math namespace

#endif /// GUARD_DY_HELPER_TYPE_MATRIX2_H