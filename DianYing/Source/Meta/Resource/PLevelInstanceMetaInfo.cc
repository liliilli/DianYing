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

#include <Dy/Meta/Resource/PLevelInstanceMetaInfo.h>
#include <nlohmann/json.hpp>
#include <Dy/Helper/Library/HelperJson.h>
#include <Dy/Helper/StringSwitch.h>

namespace dy
{

PDyLevelConstructMetaInfo PLevelInstanceMetaInfo::MakeLevelInformation(
  const PLevelInstanceMetaInfo& iMetaInfo)
{
  if (iMetaInfo.mInternalInfo.empty() == false)
  {
    const auto optJson = GetJsonAtlasFromString(iMetaInfo.mInternalInfo);
    MDY_ASSERT(optJson.has_value() == true);
    return (*optJson).get<PDyLevelConstructMetaInfo>();
  }
  else
  {
    const auto optJson = DyGetJsonAtlasFromFile(iMetaInfo.mExternalFilePath);
    return (*optJson).get<PDyLevelConstructMetaInfo>();
  }
}

void to_json(nlohmann::json& oJson, const PLevelInstanceMetaInfo& iMeta)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void from_json(const nlohmann::json& iJson, PLevelInstanceMetaInfo& oMeta)
{
  oMeta.mSourceType = EDyResourceSource::External;

  const auto loadingType = DyJsonGetValueFrom<std::string>(iJson, "LoadingType");
  switch (DyStrSwitchInput(loadingType))
  {
  case DyStrCase("Internal"):
    oMeta.mInternalInfo = iJson["Internal"].dump(0);
    break;
  case DyStrCase("External"):
    oMeta.mExternalFilePath = DyJsonGetValueFrom<std::string>(iJson, "ExternalPath");
    MDY_ASSERT_FORCE(std::filesystem::exists(oMeta.mExternalFilePath) == true);
    break;
  default: MDY_UNEXPECTED_BRANCH(); break;
  }
}

} /// ::dy namesapce