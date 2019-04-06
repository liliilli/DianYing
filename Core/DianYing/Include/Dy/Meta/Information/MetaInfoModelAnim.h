#ifndef GUARD_DY_META_INFORMATION_MetaInfoModelAnim_H
#define GUARD_DY_META_INFORMATION_MetaInfoModelAnim_H
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
#include <Dy/Meta/Information/CommonResourceMetaInfo.h>

namespace dy
{

struct PDyModelAnimInstanceMetaInfo final : public PDyCommonResourceMetaInfo
{
  struct DDetails final
  {
    float mRateScale = 1.0f; // "RateScale" Global playing rate scale of this animation sequence.
  };

  std::string mSpecifierName  = "";
  std::string mExternalPath   = "";
  std::string mSkeletonSpeicfier = "";
  bool        mIsCompressed   = false; 

  DDetails    mDetails;
};

/// @brief Serialization function
void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyModelAnimInstanceMetaInfo& p);
/// @brief Deserialization function
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyModelAnimInstanceMetaInfo& p);

/// @brief Serialization function
void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyModelAnimInstanceMetaInfo::DDetails& p);
/// @brief Deserialization function
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyModelAnimInstanceMetaInfo::DDetails& p);

} /// ::dy namespace

#endif /// GUARD_DY_META_INFORMATION_MetaInfoModelAnim_H