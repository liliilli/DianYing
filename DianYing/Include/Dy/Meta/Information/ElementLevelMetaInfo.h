#ifndef GUARD_DY_ELELMENT_DESCRIPTOR_LEVELDESCRIPTOR_H
#define GUARD_DY_ELELMENT_DESCRIPTOR_LEVELDESCRIPTOR_H
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

#include <any>
#include <vector>
#include <unordered_set>
#include <nlohmann/json_fwd.hpp>

#include <Dy/Helper/Type/ColorRGBA.h>
#include <Dy/Helper/Type/ColorRGB24.h>
#include <Dy/Meta/Type/DDyResourceName.h>
#include <Dy/Meta/Information/ElementObjectMetaInfo.h>

//!
//! Implementation
//!

namespace dy
{

///
/// @struct PDyLevelConstructMetaInfo
/// @brief Level construction descriptor.
///
struct PDyLevelConstructMetaInfo final
{
  PDyLevelConstructMetaInfo() = default;
  MDY_ONLY_MOVEABLE_PROPERTIES_DEFAULT(PDyLevelConstructMetaInfo);

  struct DMeta final
  {
    /// Initial level name
    std::string mLevelName;
    /// Initial background of scene to create
    DDyColorRGB mLevelBackgroundColor = DDyColorRGB{1, 1, 1};
  };

  /// DMeta scene common information.
  DMeta mMeta = {};
  /// Object meta information
  TObjectMetaInfoList mLevelObjectMetaInfoList = {};

  /// @brief Get level resource set.
  /// Duplicated resource speicifer on this level is shrinked into one.
  TDDyResourceNameSet GetLevelResourceSet() const noexcept;
};

void to_json  (_MINOUT_ nlohmann::json& j,    _MIN_ const PDyLevelConstructMetaInfo& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyLevelConstructMetaInfo& p);

void to_json  (_MINOUT_ nlohmann::json& j,    _MIN_ const PDyLevelConstructMetaInfo::DMeta& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyLevelConstructMetaInfo::DMeta& p);

} /// ::dy namespace

#endif /// GUARD_DY_ELELMENT_DESCRIPTOR_LEVELDESCRIPTOR_H