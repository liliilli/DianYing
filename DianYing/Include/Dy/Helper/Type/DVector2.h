#ifndef GUARD_DY_HELPER_TYPE_VECTOR2_H
#define GUARD_DY_HELPER_TYPE_VECTOR2_H
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

#include <Math/Type/Math/DVector2.h>
#include <nlohmann/json_fwd.hpp>

namespace dy
{

/// @brief Float type 2-element vector struct.
using DVec2 = math::DVector2<math::TReal>;
using DIVec2 = math::DVector2<math::TI32>;

} /// ::dy namespace

namespace dy::math
{

void to_json(nlohmann::json& j, const DVec2& p);
void from_json(const nlohmann::json& j, DVec2& p);

void to_json  (nlohmann::json& j, const DIVec2& p);
void from_json(const nlohmann::json& j, DIVec2& p);

} /// ::dy::math namespace

namespace std
{

template <> struct hash<dy::DVec2>
{
  size_t operator()(const dy::DVec2& iVertex) const 
  {
    using namespace ::dy::math;
    return ((hash<TReal>()(iVertex.X) 
          ^ (hash<TReal>()(iVertex.Y) << 1)) >> 1);
  }
};

template <> struct hash<dy::DIVec2>
{
  size_t operator()(const dy::DIVec2& iVertex) const 
  {
    using namespace ::dy::math;
    return ((hash<TI32>()(iVertex.X) 
          ^ (hash<TI32>()(iVertex.Y) << 1)) >> 1);
  }
};

} /// ::std namespace

#endif /// GUARD_DY_HELPER_TYPE_VECTOR2_H