#ifndef GUARD_DY_META_RESOURCE_RPLEVELINSTANCEMETAINFO_H
#define GUARD_DY_META_RESOURCE_RPLEVELINSTANCEMETAINFO_H
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

#include <filesystem>
#include <nlohmann/json_fwd.hpp>
#include <Dy/Meta/Information/CommonResourceMetaInfo.h>
#include <Dy/Meta/Information/ElementLevelMetaInfo.h>

namespace dy
{

/// @struct PLevelInstanceMetaInfo
/// @brief Level construction descriptor.
struct PLevelInstanceMetaInfo final : public PDyCommonResourceMetaInfo
{
  std::string           mLevelName;
  std::string           mInternalInfo;
  std::filesystem::path mExternalFilePath;

  /// @brief Make level construct info from instance meta info instance.
  [[nodiscard]] static PDyLevelConstructMetaInfo 
  MakeLevelInformation(const PLevelInstanceMetaInfo& iMetaInfo);
};

/// @brief Serialization function.
void to_json(nlohmann::json& oJson, const PLevelInstanceMetaInfo& iMeta);
/// @brief Deserialization function.
void from_json(const nlohmann::json& iJson, PLevelInstanceMetaInfo& oMeta);

} /// ::dy namespace

#endif /// GUARD_DY_META_RESOURCE_RPLEVELINSTANCEMETAINFO_H
