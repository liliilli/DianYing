#pragma once
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

#include <nlohmann/json_fwd.hpp>
#include <Math/Type/Micellanous/DBounds2D.h>
#include <Math/Type/Micellanous/DBounds3D.h>

namespace physx 
{
class PxBounds3;
}

namespace dy
{

using DBounds2D = math::DBounds2D<TReal>;
using DBounds3D = math::DBounds3D<TReal>;

struct FBounds3D final
{
  static DBounds3D CreateWith(const physx::PxBounds3& iBound);
};

} /// ::dy namespace

namespace dy::math
{
  
void to_json(nlohmann::json& oJson, const DBounds2D<TReal>& iArea);
void from_json(const nlohmann::json& iJson, DBounds2D<TReal>& oArea);

void to_json(nlohmann::json& oJson, const DBounds3D<TReal>& iArea);
void from_json(const nlohmann::json& iJson, DBounds3D<TReal>& oArea);

} /// ::dy::math namespace
