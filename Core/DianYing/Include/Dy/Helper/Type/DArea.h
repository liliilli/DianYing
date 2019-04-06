#ifndef GUARD_DY_HELPER_TYPE_AREA_H
#define GUARD_DY_HELPER_TYPE_AREA_H
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

#include <Math/Type/Micellanous/DArea2D.h>
#include <Math/Type/Micellanous/DArea3D.h>
#include <nlohmann/json_fwd.hpp>

namespace dy
{

using DArea2D = math::DArea2D<TReal>;
using DArea3D = math::DArea3D<TReal>;

} /// ::dy namespace

namespace dy::math
{

void to_json(nlohmann::json& oJson, const DArea2D<TReal>& iArea);
void from_json(const nlohmann::json& iJson, DArea2D<TReal>& oArea);

void to_json(nlohmann::json& oJson, const DArea3D<TReal>& iArea);
void from_json(const nlohmann::json& iJson, DArea3D<TReal>& oArea);

} /// ::dy::math namespace

#endif /// GUARD_DY_HELPER_TYPE_AREA_H