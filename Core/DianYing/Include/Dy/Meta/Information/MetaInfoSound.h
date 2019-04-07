#ifndef GUARD_DY_META_INFORMATION_MetaInfoSound_H
#define GUARD_DY_META_INFORMATION_MetaInfoSound_H
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

///
/// @struct PDySoundInstanceMetaInfo
/// @brief Sound meta instance information descriptor
///
struct PDySoundInstanceMetaInfo final : public dy::PDyCommonResourceMetaInfo
{
  std::string mSpecifierName = "";
  std::string mExternalPath   = "";
};

/// @brief Serialization function
void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDySoundInstanceMetaInfo& p);
/// @brief Deserialization function
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDySoundInstanceMetaInfo& p);

} /// ::dy namespace

#endif /// GUARD_DY_META_INFORMATION_MetaInfoSound_H
