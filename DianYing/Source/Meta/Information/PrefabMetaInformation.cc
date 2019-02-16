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
#include <Dy/Meta/Information/PrefabMetaInformation.h>
#include <nlohmann/json.hpp>
#include <Dy/Helper/Library/HelperJson.h>
#include <Dy/Management/SettingManager.h>

namespace dy
{

std::unique_ptr<PDyPrefabInstanceMetaInfo>
PDyPrefabInstanceMetaInfo::CreateMetaInformation(_MIN_ const nlohmann::json& itemAtlas)
{
  return std::make_unique<PDyPrefabInstanceMetaInfo>(itemAtlas.get<PDyPrefabInstanceMetaInfo>());
}

void to_json(nlohmann::json& j, const PDyPrefabInstanceMetaInfo& p)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void from_json(const nlohmann::json& j, PDyPrefabInstanceMetaInfo& p)
{
  using TDCommonProperties  = PDyPrefabInstanceMetaInfo::DCommonProperties;

  DyJsonGetValueFromTo(j, "Name",             p.mSpecifierName);
  DyJsonGetValueFromTo(j, "Type",             p.mPrefabType);
  DyJsonGetValueFromTo(j, "CommonProperties", p.mCommonProperties);
  DyJsonGetValueFromTo(j, "ComponentList",    p.mMetaComponentInfo);
}

void to_json(nlohmann::json& j, const PDyPrefabInstanceMetaInfo::DCommonProperties& p)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void from_json(const nlohmann::json& j, PDyPrefabInstanceMetaInfo::DCommonProperties& p)
{
  DyJsonGetValueFromTo(j, "ParentSpecifierName", p.mParentSpecifierName);
  DyJsonGetValueFromTo(j, "ObjectTag", p.mTagSpecifier);

  /// Validity Test
  MDY_CALL_ASSERT_SUCCESS(MDySetting::GetInstance().MDY_PRIVATE(CheckObjectTagIsExist)(p.mTagSpecifier));
}

} /// ::dy namespace