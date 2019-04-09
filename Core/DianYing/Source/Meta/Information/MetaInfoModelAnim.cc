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
#include <Dy/Meta/Information/MetaInfoModelAnim.h>
#include <Dy/Helper/Library/HelperJson.h>
#include <Dy/Helper/System/Assertion.h>

namespace dy
{

/// @brief Serialization function
void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyModelAnimInstanceMetaInfo& p)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

/// @brief Deserialization function
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyModelAnimInstanceMetaInfo& p)
{
  json::GetValueFromTo(j, "ExternalPath", p.mExternalPath);
  json::GetValueFromTo(j, "IsCompressed", p.mIsCompressed);
  json::GetValueFromTo(j, "SkeletonSpecifier", p.mSkeletonSpeicfier);
  json::GetValueFromTo(j, "Details", p.mDetails);
}

/// @brief Serialization function
void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyModelAnimInstanceMetaInfo::DDetails& p)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

/// @brief Deserialization function
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyModelAnimInstanceMetaInfo::DDetails& p)
{
  json::GetValueFromTo(j, "RateScale", p.mRateScale);
}

} /// ::dy namespace