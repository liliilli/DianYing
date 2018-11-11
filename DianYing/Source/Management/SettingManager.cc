#include <precompiled.h>
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
/// @log
/// 2018-08-21 Create file.
///

/// Header file
#include <Dy/Management/SettingManager.h>

#include <cassert>

#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
#include <cxxopts.hpp>

#include <Dy/Management/LoggingManager.h>
#include <Dy/Management/TimeManager.h>
#include <Dy/Helper/JsonHelper.h>
#include <Dy/Helper/Constant/StringSettingFile.h>

//!
//! Local translation unit variables or functions.
//!

namespace
{

MDY_SET_IMMUTABLE_STRING(sCategoryDescription,  "Description");
MDY_SET_IMMUTABLE_STRING(sCategoryGameplay,     "Gameplay");
MDY_SET_IMMUTABLE_STRING(sCategoryInput,        "Input");
MDY_SET_IMMUTABLE_STRING(sCategoryTag,          "Tag");

///
/// @brief  Get rendering api type value from argument string.
/// @param  argString Lowered graphics api argument string if not, just return Error type.
/// @return Rendering api type value.
///
MDY_NODISCARD dy::EDyRenderingApi DyGetRenderingApiType(_MIN_ const std::string& argString) noexcept
{
  /// Temporal structure to bind option string and type.
  struct DItem final
  {
    const char*           mCString  = "";
    dy::EDyRenderingApi   mType     = dy::EDyRenderingApi::NoneError;

    explicit DItem(const char* cString, dy::EDyRenderingApi type) : mCString(cString), mType(type) {};
  };

  //! - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //! FUNCTIONBODY ∨
  //! - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  static const std::vector<DItem> renderingTypeList =
  {
    DItem{"vulkan", dy::EDyRenderingApi::Vulkan},
    DItem{"opengl", dy::EDyRenderingApi::OpenGL},
    DItem{"d3d11",  dy::EDyRenderingApi::DirectX11},
    DItem{"d3d12",  dy::EDyRenderingApi::DirectX12}
  };

  // Check
  for (const auto& item : renderingTypeList)
  {
    if (argString == item.mCString) { return item.mType; }
  }

  return dy::EDyRenderingApi::NoneError;
}

} /// unnamed namespace

//!
//! Implementation
//!

namespace dy
{

EDyRenderingApi MDySetting::GetRenderingType() const noexcept
{
  return this->mRenderingType;
}

void MDySetting::SetFeatureLogging(_MIN_ bool isEnabled) noexcept
{
  if (this->mIsEnabledLogging != isEnabled)
  {
    auto& logManager = dy::MDyLog::GetInstance();
    MDY_LOG_INFO_D("{} | Logging : {}", "Feature", isEnabled ? "ON" : "OFF");
    switch (isEnabled)
    {
    case false: logManager.pfTurnOff(); break;
    case true:  logManager.pfTurnOn();  break;
    }

    this->mIsEnabledLogging = isEnabled;
  }
}

void MDySetting::SetSubFeatureLoggingToConsole(_MIN_ bool isEnabled) noexcept
{
  this->mIsEnabledLoggingToConsole = isEnabled;
  MDY_LOG_INFO_D("{} | Logging Console : {}. Need to be restart logger.", "SubFeature", isEnabled ? "ON" : "OFF");
}

void MDySetting::SetSubFeatureLoggingToFile(_MIN_ bool isEnabled) noexcept
{
  this->mIsEnabledLoggingToFile = isEnabled;
  MDY_LOG_INFO_D("{} | Logging File : {}. Need to be restart logger.", "SubFeature", isEnabled ? "ON" : "OFF");
}

void MDySetting::SetLogFilePath(_MIN_ const std::string& path) noexcept
{
  if (path.empty() == true)
  {
    MDY_LOG_ERROR_D("{} | new log file path is empty. Log file path did not change. Log file path : {}", this->mLogFilePath);
  }
  else
  {
    this->mLogFilePath = path;
    MDY_LOG_INFO_D("{} | Update Log file path : {}. Need to be restart logger.", "SubFeature", this->mLogFilePath);
  }
}

void MDySetting::SetGlobalDefaultShadowMapResolution(_MIN_ const DDyVector2& size) noexcept
{
  if (size.X <= 0 || size.Y <= 0) { return; }
  this->mGamePlay.mShadow.mShadowGlobalDefaultMap = DDyVectorInt2{size};
}

bool MDySetting::IsVSyncEnabled() const noexcept
{
  return this->mIsEnabledVsync;
}

void MDySetting::SetVSyncMode(bool enableVsync) noexcept
{
  if (this->mIsEnabledVsync != enableVsync)
  {
    this->mIsEnabledVsync = enableVsync;
    if (MDyTime::IsInitialized() && enableVsync)
    {
      MDyTime::GetInstance().pfSetVsync(enableVsync);
    }
  }
}

void MDySetting::pSetupExecutableArgumentSettings()
{
  ///
  /// @brief Setup rendering api type from argument.
  /// @param result ["graphics"] Option value from parsing library.
  ///
  static auto SetupRenderingType = [this](const cxxopts::OptionValue& result)
  { // Lower api string.
    std::string graphicsApi = result.as<std::string>();
    std::transform(graphicsApi.begin(), graphicsApi.end(), graphicsApi.begin(), ::tolower);

    // Get Graphics api string.
    this->mRenderingType = DyGetRenderingApiType(graphicsApi);
    MDY_ASSERT(
        this->mRenderingType != EDyRenderingApi::NoneError,
        "Rendering api option is not specified properly. Must be \"OpenGL\".");
  };

  ///
  /// @brief Setup feature logging to console from argument.
  /// @param result ["enable_logging_console"] Option value from parsing library.
  ///
  static auto SetupLoggingConsoleFeature = [this](const cxxopts::OptionValue& result)
  {
    if (result.as<bool>() == true)
    {
      this->mIsEnabledLogging           = true;
      this->mIsEnabledLoggingToConsole  = true;
    }
  };

  ///
  /// @brief Setup feature logging to file from argument.
  /// @param result ["enable_logging_file"] Option value from parsing library.
  ///
  static auto SetupLoggingFileFeature = [this](const cxxopts::OptionValue& result)
  {
    if (std::string f = result.as<std::string>(); f.empty() == false)
    {
      this->mIsEnabledLogging = true;
      this->mIsEnabledLoggingToFile = true;

      if (f.find_last_of('.') == std::string::npos) { f += ".txt"; }
      this->mLogFilePath = f;
    }
  };

  //! - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //! FUNCTIONBODY ∨
  //! - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  MDY_ASSERT(
      this->mIsInitialized == false,
      "MDySetting::pSetupExecutableArgumentSettings must be called only before Initialization.");

  cxxopts::Options options("Dy", "Dy game framework");
  options.add_options()
      ("g,graphics",                "Enable graphics API with", cxxopts::value<std::string>())
      ("c,enable_logging_console",  "Enable logging console",   cxxopts::value<bool>())
      ("f,enable_logging_file",     "Enable logging file to",   cxxopts::value<std::string>())
  ;

  #if defined(MDY_PLATFORM_FLAG_WINDOWS) && defined(_WIN32)
    const auto result = options.parse(__argc, __argv);
  #elif defined(MDY_PLATFORM_FLAG_LINUX) && defined(__linux__)
    static_assert(false, "Linux does not support now.");
  #elif defined(MDY_PLATFORM_FLAG_MACOS)
    static_assert(false, "Macos does not support now.");
  #endif

  SetupRenderingType        (result["graphics"]);
  SetupLoggingConsoleFeature(result["enable_logging_console"]);
  SetupLoggingFileFeature   (result["enable_logging_file"]);
}

EDySuccess MDySetting::pfInitialize()
{
  ///
  /// @function InitializeGraphicsApi
  /// @brief Initialize graphics api dependencies.
  ///
  static auto InitializeGraphicsApi = [](MDySetting& manager) -> EDySuccess
  { // Set rendering api type.
    if (manager.GetRenderingType() == EDyRenderingApi::NoneError) { return DY_FAILURE; }

    switch (manager.mRenderingType)
    {
    case EDyRenderingApi::Vulkan:
      MDY_NOT_IMPLEMENTED_ASSERT();
      MDY_LOG_INFO_D("{} | Graphics API : {}", "Feature", "Vulkan");
      break;
    case EDyRenderingApi::DirectX11:
      MDY_NOT_IMPLEMENTED_ASSERT();
      MDY_LOG_INFO_D("{} | Graphics API : {}", "Feature", "DirectX11");
      break;
    case EDyRenderingApi::DirectX12:
      MDY_NOT_IMPLEMENTED_ASSERT();
      MDY_LOG_INFO_D("{} | Graphics API : {}", "Feature", "DirectX12");
      break;
    case EDyRenderingApi::OpenGL:
      MDY_LOG_INFO_D("{} | Graphics API : {}", "Feature", "OpenGL");
      break;
    default:
      MDY_UNEXPECTED_BRANCH();
      MDY_LOG_INFO_D("{} | Graphics API : {}", "Feature", "Unknown");
      break;
    }

    return DY_SUCCESS;
  };

  //! - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //! FUNCTIONBODY ∨
  //! - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // Output setting options at debug mode.
  MDY_LOG_INFO_D("{} | MDySetting::pfInitialize().",          "FunctionCall");
  MDY_LOG_INFO_D("{} | Logging : {}", "Feature",              this->mIsEnabledLogging ? "ON" : "OFF");
  MDY_LOG_INFO_D("{} | Logging Console : {}", "SubFeature",   this->mIsEnabledLoggingToConsole ? "ON" : "OFF");
  MDY_LOG_INFO_D("{} | Logging File : {}", "SubFeature",      this->mIsEnabledLoggingToFile ? "ON" : "OFF");
  MDY_LOG_INFO_D("{} | Logging File path : {}", "SubFeature", this->mLogFilePath);
  MDY_LOG_INFO_D("{} | Vsync : {}", "Feature",                this->mIsEnabledVsync ? "ON" : "OFF");

  if (InitializeGraphicsApi(*this) == DY_FAILURE) { return DY_FAILURE; }

  const auto opSettingAtlas = DyGetJsonAtlas(MSVSTR(gSettingPathName));
  MDY_ASSERT(opSettingAtlas.has_value() == true, "Failed to open application setting file.");

  { // Apply setting to project before everthing starts to working.
    const auto& settingAtlas = opSettingAtlas.value();
    this->mDescription  = DyGetValue<decltype(this->mDescription)>(settingAtlas, sCategoryDescription);
    this->mGamePlay     = DyGetValue<decltype(this->mGamePlay)>   (settingAtlas, sCategoryGameplay);
    this->mInput        = DyGetValue<decltype(this->mInput)>      (settingAtlas, sCategoryInput);
    this->mTag          = DyGetValue<decltype(this->mTag)>        (settingAtlas, sCategoryTag);
  }

  this->mIsInitialized = true;
  return DY_SUCCESS;
}

EDySuccess MDySetting::pfRelease()
{
  MDY_LOG_INFO_D("{} | MDySetting::pfRelease()", "Function call");
  return DY_SUCCESS;
}

void to_json(nlohmann::json& j, const MDySetting& p)
{

}

void from_json(const nlohmann::json& j, MDySetting& p)
{

}

} /// ::dy namespace