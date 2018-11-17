#ifndef GUARD_DY_ELELMENT_DESCRIPTOR_LEVELDESCRIPTOR_H
#define GUARD_DY_ELELMENT_DESCRIPTOR_LEVELDESCRIPTOR_H
///
/// MIT License
/// Copyright (c) 2018 Jongmin Yun
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
#include <nlohmann/json_fwd.hpp>

#include <Dy/Helper/Type/ColorRGBA.h>
#include <Dy/Helper/Type/ColorRGB24.h>
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
    std::string   mLevelName                    = MDY_INITIALIZE_EMPTYSTR;
    /// Is Using UUID For Specification
    bool          mIsUsingUUIDForSpecification  = false;
    /// Initial background of scene to create
    DDyColorRGB24 mLevelBackgroundColor         = {255, 255, 255};
  };

  using TObjectMetaInfoList = std::vector<std::unique_ptr<PDyObjectMetaInfo>>;

  /// DMeta scene common information.
  DMeta               mMetaCategory             = {};
  /// Object meta information
  TObjectMetaInfoList mLevelObjectMetaInfoList  = {};
};

void to_json  (_MINOUT_ nlohmann::json& j,    _MIN_ const PDyLevelConstructMetaInfo& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyLevelConstructMetaInfo& p);

void to_json  (_MINOUT_ nlohmann::json& j,    _MIN_ const PDyLevelConstructMetaInfo::DMeta& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyLevelConstructMetaInfo::DMeta& p);

void to_json  (_MINOUT_ nlohmann::json& j,    _MIN_ const PDyLevelConstructMetaInfo::TObjectMetaInfoList& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyLevelConstructMetaInfo::TObjectMetaInfoList& p);

} /// ::dy namespace

#endif /// GUARD_DY_ELELMENT_DESCRIPTOR_LEVELDESCRIPTOR_H