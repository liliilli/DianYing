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
#include <Dy/Helper/Type/DArea3D.h>
#include <Dy/Helper/Library/HelperJson.h>
#include <Dy/Helper/Type/DVector3.h>

namespace dy::math
{

void to_json(nlohmann::json& oJson, const DArea3D<TReal>& iArea)
{
  oJson = nlohmann::json
  {
    {"Start", iArea.GetStartPoint()},
    {"Length", iArea.GetWhd()}
  };
}

void from_json(const nlohmann::json& iJson, DArea3D<TReal>& oArea)
{
  oArea.SetStartPoint(json::GetValueFrom<DVec3>(iJson, "Start"));
  oArea.SetWhd(json::GetValueFrom<DVec3>(iJson, "Length"));
}

} /// ::dy namespace