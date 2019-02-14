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
#include <Dy/Helper/MCS/Functions.h>

#include <filesystem>
#include <Dy/Helper/Library/HelperRegex.h>
#include <Dy/Helper/Library/HelperJson.h>
#include "Dy/Management/Type/SettingContainer.h"

namespace dy::mcs
{

void Compress(_MIN_ const std::filesystem::path& iPath, _MIN_ bool iOverride)
{
  // If running mode is compress data, do the thing and return.
  // `MDY_FLAG_MODE_POPULATE_COMPRESSED_DATAFILE` is defined, do not run application 
  // but compression mode will be operated.
  namespace fs = std::filesystem;
  fs::create_directory("./data");

  // If Override disbled, try to check there is already data file.
  if (iOverride == false)
  {
    for (const auto& entry : fs::directory_iterator("./data"))
    {
      if (entry.is_regular_file() == false) { continue; }
      if (entry.path().has_filename() == false) { continue; }

      const auto flag = dy::DyRegexCheckIsCompressedDataFile(entry.path().filename().string());
      MDY_ASSERT(flag == false, R"(There is already any Data###.dydat file in "./data" directory. Compression failed.)");
    }
  }

  const auto opSettingAtlas = DyGetJsonAtlasFromFile(iPath.string());
  MDY_ASSERT(opSettingAtlas.has_value() == true, "Failed to open application setting file.");

  DDySettingMetaPath mDevMetaPath = {};
  DyJsonGetValueFromTo(opSettingAtlas.value(), "MetaPath", mDevMetaPath);
  static constexpr std::string_view kMetaPath = "./data/Data000.dydat";
  
  {
    std::ofstream o(kMetaPath);
    nlohmann::json j;
    j["Setting"] = opSettingAtlas.value();

    { // Font container.
      const auto opt = dy::DyGetJsonAtlasFromFile(mDevMetaPath.mFontMetaPath);
      MDY_ASSERT(opt.has_value() == true, "Failed to open font file.");
      j["Font"] = opt.value();
    }
    { // Prefab container.
      const auto opt = dy::DyGetJsonAtlasFromFile(mDevMetaPath.mPrefabMetaPath);
      MDY_ASSERT(opt.has_value() == true, "Failed to open prefab file.");
      j["Prefab"] = opt.value();
    }
    { // Shader container.
      const auto opt = dy::DyGetJsonAtlasFromFile(mDevMetaPath.mGLShaderMetaPath);
      MDY_ASSERT(opt.has_value() == true, "Failed to open shader meta file.");
      j["Shader"] = opt.value();
    }
    { // Material container.
      const auto opt = dy::DyGetJsonAtlasFromFile(mDevMetaPath.mMaterialMetaPath);
      MDY_ASSERT(opt.has_value() == true, "Failed to open material meta file.");
      j["Material"] = opt.value();
    }
    { // Model animation container.
      const auto opt = dy::DyGetJsonAtlasFromFile(mDevMetaPath.mModelAnimMetaPath);
      MDY_ASSERT(opt.has_value() == true, "Failed to open model animation meta file.");
      j["ModelAnim"] = opt.value();
    }
    { // Model mesh container.
      const auto opt = dy::DyGetJsonAtlasFromFile(mDevMetaPath.mModelMeshMetaPath);
      MDY_ASSERT(opt.has_value() == true, "Failed to open model mesh meta file.");
      j["ModelMesh"] = opt.value();
    }
    { // Model container.
      const auto opt = dy::DyGetJsonAtlasFromFile(mDevMetaPath.mModelMetaPath);
      MDY_ASSERT(opt.has_value() == true, "Failed to open model meta file.");
      j["Model"] = opt.value();
    }
    { // Model skeleton container.
      const auto opt = dy::DyGetJsonAtlasFromFile(mDevMetaPath.mModelSkeletonMetaPath);
      MDY_ASSERT(opt.has_value() == true, "Failed to open model skeleton meta file.");
      j["ModelSkel"] = opt.value();
    }
    { // Sound container.
      const auto opt = dy::DyGetJsonAtlasFromFile(mDevMetaPath.mSoundMetaPath);
      MDY_ASSERT(opt.has_value() == true, "Failed to open sound meta file.");
      j["Sound"] = opt.value();
    }
    { // Texture container.
      const auto opt = dy::DyGetJsonAtlasFromFile(mDevMetaPath.mTextureMetaPath);
      MDY_ASSERT(opt.has_value() == true, "Failed to open texture meta file.");
      j["Texture"] = opt.value();
    }
    { // Level container.
      const auto opt = dy::DyGetJsonAtlasFromFile(mDevMetaPath.mSceneMetaPath);
      MDY_ASSERT(opt.has_value() == true, "Failed to open level meta file.");
      j["Level"] = opt.value(); 
    }
    { // Script container.
      const auto opt = dy::DyGetJsonAtlasFromFile(mDevMetaPath.mScriptMetaPath);
      MDY_ASSERT(opt.has_value() == true, "Failed to open script meta file.");
      j["Script"] = opt.value();
    }
    { // Widget container.
      const auto opt = dy::DyGetJsonAtlasFromFile(mDevMetaPath.mWidgetMetaPath);
      MDY_ASSERT(opt.has_value() == true, "Failed to open widget meta file.");
      j["Widget"] = opt.value();
    }

    o << j;
    o.close();
  }
}

} /// ::dy::mcs namespace