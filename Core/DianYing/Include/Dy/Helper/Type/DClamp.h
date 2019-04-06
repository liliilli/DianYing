#ifndef GUARD_DY_HELPER_TYPE_CLAMP_H
#define GUARD_DY_HELPER_TYPE_CLAMP_H
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

#include <nlohmann/json.hpp>
#include <Math/Type/Micellanous/DClamp.h>

namespace dy
{

using math::DClamp;

} /// ::dy namespace

namespace dy::math
{

template <typename TType, TI64 TStart, TI64 TEnd>
void to_json(nlohmann::json& j, const DClamp<TType, TStart, TEnd>& p)
{
  j = p();
}

template <typename TType, TI64 TStart, TI64 TEnd>
void from_json(const nlohmann::json& j, DClamp<TType, TStart, TEnd>& p)
{
  p = j.get<TType>();
}

} /// ::dy::math namespace

#endif /// GUARD_DY_HELPER_TYPE_CLAMP_H
