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
#include <Dy/Meta/Information/Physics/DColliderMetaInfo.h>
#include <Dy/Helper/Library/HelperJson.h>
#include <nlohmann/json.hpp>

namespace dy
{

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDySphereColliderMetaInfo& p)
{
  json::GetValueFromTo(j, "Radius", p.mRadius);
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyCapsulesColliderMetaInfo& p)
{
  json::GetValueFromTo(j, "HalfHeight", p.mHalfHeight);
  json::GetValueFromTo(j, "Radius", p.mRadius);
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyBoxColliderMetaInfo& p)
{
  json::GetValueFromTo(j, "HalfExtent", p.mHalfExtent);
}

} /// ::dy namespace