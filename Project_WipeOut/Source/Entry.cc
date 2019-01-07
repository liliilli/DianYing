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

#include <Dy/Include/Entry.h>

/// @brief Main entry function of WIN32 platforms.
#if defined(MDY_PLATFORM_FLAG_WINDOWS)
namespace
{

HINSTANCE ghInstance      = nullptr;
HINSTANCE ghPrevInstance  = nullptr;
LPSTR     gpCmdLine;
int       gnCmdShow;

///
/// @brief Turn on memory leak detection feature and console window for logging.
///
EDySuccess MDY_PRIVATE_SPECIFIER(DyInitializeWin32Debug)()
{
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
  _CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG );
  return DY_SUCCESS;
}

} /// unname namespace

#if (defined(_DEBUG) == true) || (defined(NDEBUG) == false)
#define MDY_WIN32_TRY_TURN_ON_DEBUG()   MDY_CALL_ASSERT_SUCCESS(MDY_PRIVATE_SPECIFIER(DyInitializeWin32Debug)())
#define MDY_WIN32_TRY_TURN_OFF_DEBUG()  (void)0
#else
#define MDY_WIN32_TRY_TURN_ON_DEBUG()   (void)0
#define MDY_WIN32_TRY_TURN_OFF_DEBUG()  (void)0
#endif

/// @brief Main function of win32 / win64 platform.
int APIENTRY WinMain(_MIN_ HINSTANCE hInstance, _MIN_ HINSTANCE hPrevInstance, _MIN_ LPSTR pCmdLine, _MIN_ int nCmdShow)
{
  ghInstance      = hInstance;
  ghPrevInstance  = hPrevInstance;
  gpCmdLine       = pCmdLine;
  gnCmdShow       = nCmdShow;

  MDY_WIN32_TRY_TURN_ON_DEBUG();
  #if defined(MDY_FLAG_MODE_POPULATE_COMPRESSED_DATAFILE) == false
  { // If `MDY_FLAG_MODE_POPULATE_COMPRESSED_DATAFILE` is not defined, just run application.
    MDY_CALL_ASSERT_SUCCESS(dy::DyEngine::Initialize());
    dy::DyEngine::GetInstance()();
    MDY_CALL_ASSERT_SUCCESS(dy::DyEngine::Release());
  }
  #else
  { // `MDY_FLAG_MODE_POPULATE_COMPRESSED_DATAFILE` is defined, do not run application 
    // but compression mode will be operated.
    namespace fs = std::filesystem;
    MDY_ASSERT(fs::exists(M_PATH_PLAIN_PATH_OF_SETTING_JSON) == true, "Entry setting file is not exist on given path.");
    fs::create_directory("./data");

    for (const fs::directory_entry& entry : fs::directory_iterator("./data"))
    {
      if (entry.is_regular_file() == false) { continue; }
      if (entry.path().has_filename() == false) { continue; }

      const auto flag = dy::DyRegexCheckIsCompressedDataFile(entry.path().filename().string());
      MDY_ASSERT(flag == false, R"(There is already any Data###.dydat file in "./data" directory. Compression failed.)");
    }

    MDY_CALL_ASSERT_SUCCESS(dy::MDySetting::Initialize());
    const auto& settingManager = dy::MDySetting::GetInstance();
    static constexpr std::string_view kMetaPath = "./data/Data000.dydat";
    
    {
      std::ofstream o(kMetaPath);
      nlohmann::json j;
      {
        const auto optSetting = dy::DyGetJsonAtlasFromFile(M_PATH_PLAIN_PATH_OF_SETTING_JSON);
        MDY_ASSERT(optSetting.has_value() == true, "Failed to open setting file.");
        j["Setting"] = optSetting.value();
      }

      const auto& pathInfo = settingManager.GetMetaPathSettingInformation();
      { // Font container.
        const auto opt = dy::DyGetJsonAtlasFromFile(pathInfo.mFontMetaPath);
        MDY_ASSERT(opt.has_value() == true, "Failed to open font file.");
        j["FontContainer"] = opt.value();
      }
      { // Prefab container.
        const auto opt = dy::DyGetJsonAtlasFromFile(pathInfo.mPrefabMetaPath);
        MDY_ASSERT(opt.has_value() == true, "Failed to open prefab file.");
        j["PrefabContainer"] = opt.value();
      }
      { // Widget container.
        const auto opt = dy::DyGetJsonAtlasFromFile(pathInfo.mWidgetMetaPath);
        MDY_ASSERT(opt.has_value() == true, "Failed to open widget meta file.");
        j["WidgetContainer"] = opt.value();
      }
      { // Texture container.
        j["TextureContainer"] = nlohmann::json{{}};
      }
      { // Shader container.
        j["ShaderContainer"] = nlohmann::json{{}};
      }
      { // Model container.
        j["ModelContainer"] = nlohmann::json{{}};
      }
      { // Sound container.
        j["SoundContainer"] = nlohmann::json{{}};
      }
      { // Script container.
        const auto opt = dy::DyGetJsonAtlasFromFile(pathInfo.mScriptMetaPath);
        MDY_ASSERT(opt.has_value() == true, "Failed to open script meta file.");
        j["ScriptContainer"] = opt.value();
      }
      { // Level container.
        const auto opt = dy::DyGetJsonAtlasFromFile(pathInfo.mSceneMetaPath);
        MDY_ASSERT(opt.has_value() == true, "Failed to open level meta file.");
        j["LevelContainer"] = opt.value(); 
      }

      o << j;
      o.close();
    }

    MDY_CALL_ASSERT_SUCCESS(dy::MDySetting::Release());
  }
  #endif
  MDY_WIN32_TRY_TURN_OFF_DEBUG();
  return 0;
}
#elif defined(MDY_PLATFORM_FLAG_LINUX)
int main() { return 0; }
#elif defined(MDY_PLATFORM_FLAG_MACOS)
int main() { return 0; }
#endif