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
/// @log
/// 2018-08-21 Create file.
///

/// Header file
#include <Dy/Management/MSetting.h>

#include <cassert>
#include <filesystem>

#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
#include <cxxopts.hpp>

#include <Dy/Helper/Library/HelperJson.h>
#include <Dy/Management/MLog.h>
#include <Dy/Management/MTime.h>
#include <Dy/Management/IO/MIOMeta.h>

//!
//! Local translation unit variables or functions.
//!

namespace
{

MDY_SET_IMMUTABLE_STRING(sCategoryDescription,  "Description");
MDY_SET_IMMUTABLE_STRING(sCategoryGameplay,     "Gameplay");
MDY_SET_IMMUTABLE_STRING(sCategoryInput,        "Input");
MDY_SET_IMMUTABLE_STRING(sCategoryTag,          "Tag");
MDY_SET_IMMUTABLE_STRING(sCategoryMetaPath,     "MetaPath");

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

EDyRenderingApi MSetting::GetRenderingType() const noexcept
{
  return this->mRenderingType;
}

void MSetting::SetDefaultShadowOption(bool iFlag) noexcept
{
  auto& previousFlag = this->mGamePlay.mGraphics.mIsEnabledDefaultShadow;
  if (iFlag != previousFlag)
  {
    previousFlag = iFlag;
  }
}

bool MSetting::IsDefaultShadowOptionActivated() const noexcept
{
  return this->mGamePlay.mGraphics.mIsEnabledDefaultShadow;
}

void MSetting::SetDefaultSsaoOption(bool iFlag) noexcept
{
  auto& previousFlag = this->mGamePlay.mGraphics.mIsEnabledDefaultSsao;
  if (iFlag != previousFlag)
  {
    previousFlag = iFlag;
  }
}

bool MSetting::IsDefaultSsaoOptionActivated() const noexcept
{
  return this->mGamePlay.mGraphics.mIsEnabledDefaultSsao;
}

bool MSetting::IsDebugMode() const noexcept
{
  return this->mIsDebugMode;
}

TI32 MSetting::GetWindowSizeWidth() const noexcept
{
  return this->mGamePlay.mInitialResolution.X;
}

TI32 MSetting::GetWindowSizeHeight() const noexcept
{
  return this->mGamePlay.mInitialResolution.Y;
}

const std::string& MSetting::GetInitialSceneInformationName() const noexcept
{
  return this->mGamePlay.mInitialSceneSpecifier;
}

const DIVec2& MSetting::GetGlobalDefaultShadowMapResolution() const noexcept
{
  return this->mGamePlay.mShadow.mShadowGlobalDefaultMap;
}

const DDySettingInput& MSetting::GetInputSettingInformation() const noexcept
{
  return this->mInput;
}

const DDySettingGameplay& MSetting::GetGameplaySettingInformation() const noexcept
{
  return this->mGamePlay;
}

const DDySettingMetaPath& MSetting::GetMetaPathSettingInformation() const noexcept
{
  return this->mDevMetaPath;
}

void MSetting::SetFeatureLogging(bool isEnabled) noexcept
{
  if (this->mIsEnabledLogging != isEnabled)
  {
    auto& logManager = MLog::GetInstance();
    DyPushLogDebugInfo("{} | Logging : {}", "Feature", isEnabled ? "ON" : "OFF");
    switch (isEnabled)
    {
    case false: logManager.pfTurnOff(); break;
    case true:  logManager.pfTurnOn();  break;
    }

    this->mIsEnabledLogging = isEnabled;
  }
}

void MSetting::SetSubFeatureLoggingToConsole(bool isEnabled) noexcept
{
  this->mIsEnabledLoggingToConsole = isEnabled;
  DyPushLogDebugInfo(
    "{} | Logging Console : {}. Need to be restart logger.", 
    "SubFeature", isEnabled ? "ON" : "OFF");
}

void MSetting::SetSubFeatureLoggingToFile(bool isEnabled) noexcept
{
  this->mIsEnabledLoggingToFile = isEnabled;
  DyPushLogDebugInfo(
    "{} | Logging File : {}. Need to be restart logger.", 
    "SubFeature", isEnabled ? "ON" : "OFF");
}

void MSetting::SetLogFilePath(const std::string& path) noexcept
{
  if (path.empty() == true)
  {
    DyPushLogDebugError(
      "New log file path is empty. Log file path did not change. Log file path : {}", 
      this->mLogFilePath);
  }
  else
  {
    this->mLogFilePath = path;
    DyPushLogDebugInfo(
      "{} | Update Log file path : {}. Need to be restart logger.", 
      "SubFeature", this->mLogFilePath);
  }
}

void MSetting::SetGlobalDefaultShadowMapResolution(const DVec2& size) noexcept
{
  if (size.X <= 0 || size.Y <= 0) { return; }
  this->mGamePlay.mShadow.mShadowGlobalDefaultMap = DIVec2{size};
}

bool MSetting::IsEnabledVSync() const noexcept
{
  return this->mIsEnabledVsync;
}

bool MSetting::IsEnabledFeatureLogging() const noexcept
{
  return this->mIsEnabledLogging;
}

bool MSetting::IsEnabledSubFeatureLoggingToConsole() const noexcept
{
  return this->mIsEnabledLoggingToConsole;
}

bool MSetting::IsEnableSubFeatureLoggingToFile() const noexcept
{
  return this->mIsEnabledLoggingToFile;
}

EDyAppMode MSetting::GetApplicationMode() const noexcept
{
  return this->mApplicationMode;
}

const std::string& MSetting::GetLogFilePath() const noexcept
{
  return this->mLogFilePath;
}

void MSetting::SetVSyncMode(bool enableVsync) noexcept
{
  if (this->mIsEnabledVsync != enableVsync)
  {
    this->mIsEnabledVsync = enableVsync;
    if (MTime::IsInitialized() && enableVsync)
    {
      MTime::GetInstance().pfSetVsync(enableVsync);
    }
  }
}

EDySuccess MSetting::MDY_PRIVATE(CheckObjectTagIsExist)(const std::string& iSpecifiedTag) const noexcept
{
  if (iSpecifiedTag.empty() == true) { return DY_SUCCESS; }
  for (const auto& tagSpecifier : this->mTag.mObjectTag)
  {
    if (tagSpecifier == iSpecifiedTag) { return DY_SUCCESS; }
  }

  return DY_FAILURE;
}

const std::string& MSetting::MDY_PRIVATE(GetEntrySettingFile)() const noexcept
{
  return this->mEntrySettingPath;
}

const DDySettingSound& MSetting::GetSoundSetting() const noexcept
{
  return this->mSound;
}

const DDySettingPhysics& MSetting::GetPhysicsSetting() const noexcept
{
  return this->mPhysics;
}

void MSetting::SetRenderingMode(ERenderingModelMode iNewMode) noexcept
{
  this->mModelRenderingMode = iNewMode;
}

ERenderingModelMode MSetting::GetRenderingMode() const noexcept
{
  return this->mModelRenderingMode;
}

void MSetting::SetRenderingPhysicsCollisionShape(bool iIsEnabled) noexcept
{
  this->mIsRenderPhysicsCollisionShape = iIsEnabled;
}

bool MSetting::IsRenderPhysicsCollisionShape() const noexcept
{
  return this->mIsRenderPhysicsCollisionShape;
}

void MSetting::SetRenderingPhysicsCollisionAABB(bool iIsEnabled) noexcept
{
  this->mIsRenderPhysicsCollisionAABB = iIsEnabled;
}

bool MSetting::IsRenderPhysicsCollisionAABB() const noexcept
{
  return this->mIsRenderPhysicsCollisionAABB;
}

void MSetting::pSetupExecutableArgumentSettings()
{
  /// @brief Setup rendering api type from argument.
  /// @param result ["graphics"] Option value from parsing library.
  static auto SetupRenderingType = [this](const cxxopts::OptionValue& result)
  { // Lower api string.
    std::string graphicsApi = result.as<std::string>();
    std::transform(graphicsApi.begin(), graphicsApi.end(), graphicsApi.begin(), ::tolower);

    // Get Graphics api string.
    this->mRenderingType = DyGetRenderingApiType(graphicsApi);
    // If rendering type is none, default value is OpenGL.
    if (this->mRenderingType == EDyRenderingApi::NoneError)
    { 
      this->mRenderingType = EDyRenderingApi::OpenGL; 
    }
  };

  /// @brief Setup feature logging to console from argument.
  /// @param result ["enable_logging_console"] Option value from parsing library.
  static auto SetupLoggingConsoleFeature = [this](const cxxopts::OptionValue& result)
  {
    if (result.as<bool>() == true)
    {
      this->mIsEnabledLogging           = true;
      this->mIsEnabledLoggingToConsole  = true;
    }
  };

  static auto SetupDyDebugMode = [this](const cxxopts::OptionValue& result) { this->mIsDebugMode = result.as<bool>(); };

  /// @brief Setup feature logging to file from argument.
  /// @param result ["enable_logging_file"] Option value from parsing library.
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

  MDY_ASSERT_MSG(
      this->mIsInitialized == false,
      "MSetting::pSetupExecutableArgumentSettings must be called only before Initialization.");

  cxxopts::Options options("Dy", "Dy game framework");
  options.add_options()
      ("g,graphics",                "Enable graphics API with", cxxopts::value<std::string>()->default_value(""))
      ("c,enable_logging_console",  "Enable logging console",   cxxopts::value<bool>())
      ("f,enable_logging_file",     "Enable logging file to",   cxxopts::value<std::string>()->default_value(""))
#ifdef false
      // -m and -r can not be existed on same time.
      // Must specify setting json data path from executable application file.
      ("m,mode_compression_data", 
          "Run application as compression data mode.", 
          cxxopts::value<std::string>()->default_value(""))
#endif
      // If -r is not setup, specified compression file will be loaded.
      // Must specify setting json data path from executable application file.
      ("r,run_separated_data",    
          "Run with seperated data file, must be setting file path.", 
          cxxopts::value<std::string>()->default_value(""))
      // If -d setup, debug mode will be setup.
      // Specified keyboard key will be setup (F1 ~ F12) and override given game runtime key. 
      // and, imgui will also be initiated to see informations.
      // when -d set up, all resources will be loaded like -r flag. and saved as saparated files.
      /// @WARNING IMGUI does not follow any design-patterns like a MVC, MVP, MVVM, prism...
      /// IMGUI is just for intermediate gui rendering.
      ("d,debug",
          "Debug mode. User can make game resources with debug mode or can see informations.",
          cxxopts::value<bool>())
  ;

  #if defined(MDY_PLATFORM_FLAG_WINDOWS) && defined(_WIN32)
  {
    const auto result = options.parse(__argc, __argv);

    SetupRenderingType(result["graphics"]);
    
    SetupLoggingConsoleFeature(result["enable_logging_console"]);
    SetupLoggingFileFeature(result["enable_logging_file"]);

    { // Run seperated data.
      if (const auto r = result["run_separated_data"].as<std::string>(); r.empty() == false)
      {
        namespace fs = std::filesystem;
        MDY_ASSERT_MSG_FORCE(
            this->mFileLoadingMode == EDyFileLoadingMode::LoadCompressedFile,
            "Application running mode should not be duplicated by any mode flag.");
        MDY_ASSERT_MSG_FORCE(fs::exists(r) == true, "Compressed data entry setting file is not exist on given path.");

        this->mEntrySettingPath = r;
        this->mFileLoadingMode  = EDyFileLoadingMode::LoadSeperatedFile;
      }
    }

    // Check debug mode must be enabled.
    SetupDyDebugMode(result["debug"]);
  }
  #elif defined(MDY_PLATFORM_FLAG_LINUX) && defined(__linux__)
  {
    static_assert(false, "Linux does not support now.");
  }
  #elif defined(MDY_PLATFORM_FLAG_MACOS)
  {
    static_assert(false, "Macos does not support now.");
  }
  #endif
}

EDySuccess MSetting::pfInitialize()
{
  /// @function InitializeGraphicsApi
  /// @brief Initialize graphics api dependencies.
  static auto InitializeGraphicsApi = [](MSetting& manager) -> EDySuccess
  { // Set rendering api type.
    switch (manager.mRenderingType)
    {
    case EDyRenderingApi::Vulkan:     MDY_NOT_IMPLEMENTED_ASSERT(); break;
    case EDyRenderingApi::DirectX11:  MDY_NOT_IMPLEMENTED_ASSERT(); break;
    case EDyRenderingApi::DirectX12:  MDY_NOT_IMPLEMENTED_ASSERT(); break;
    case EDyRenderingApi::OpenGL:     DyPushLogDebugInfo("{} | Graphics API : {}", "Feature", "OpenGL"); break;
    default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(DY_FAILURE); 
    }
    return DY_SUCCESS;
  };

  //! - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //! FUNCTIONBODY ∨
  //! - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // Output setting options at debug mode.
  DyPushLogDebugInfo("{} | MSetting::pfInitialize().",          "FunctionCall");
  DyPushLogDebugInfo("{} | Logging : {}", "Feature",              this->mIsEnabledLogging ? "ON" : "OFF");
  DyPushLogDebugInfo("{} | Logging Console : {}", "SubFeature",   this->mIsEnabledLoggingToConsole ? "ON" : "OFF");
  DyPushLogDebugInfo("{} | Logging File : {}", "SubFeature",      this->mIsEnabledLoggingToFile ? "ON" : "OFF");
  DyPushLogDebugInfo("{} | Logging File path : {}", "SubFeature", this->mLogFilePath);
  DyPushLogDebugInfo("{} | Vsync : {}", "Feature",                this->mIsEnabledVsync ? "ON" : "OFF");
  MDY_CALL_ASSERT_SUCCESS(InitializeGraphicsApi(*this));

  if (this->mFileLoadingMode == EDyFileLoadingMode::LoadSeperatedFile) 
  { // If Application loading mode is `Load separated file` like a json, dydat.
    const auto opSettingAtlas = json::GetAtlasFromFile(this->mEntrySettingPath);
    MDY_ASSERT_MSG(opSettingAtlas.has_value() == true, "Failed to open application setting file.");
    const auto& settingAtlas = opSettingAtlas.value();

    // Apply setting to project before everthing starts to working.
    json::GetValueFromTo(settingAtlas, sCategoryDescription, this->mDescription);
    json::GetValueFromTo(settingAtlas, sCategoryGameplay, this->mGamePlay);
    json::GetValueFromTo(settingAtlas, sCategoryInput, this->mInput);
    json::GetValueFromTo(settingAtlas, sCategoryTag, this->mTag);
    json::GetValueFromTo(settingAtlas, "Sound", this->mSound);
    json::GetValueFromTo(settingAtlas, "Physics", this->mPhysics);

    json::GetValueFromTo(settingAtlas, sCategoryMetaPath, this->mDevMetaPath);
    MIOMeta::GetInstance().MDY_PRIVATE(InitiateMetaInformation)();
  }
  else if (this->mFileLoadingMode == EDyFileLoadingMode::LoadCompressedFile)
  { // If Application loading mode is `Load compressed file` like a `Data###.dydat`.
    MDY_ASSERT_MSG(std::filesystem::exists(this->mEntrySettingPath) == true, "Data file is not exist.");

    const auto opMetaInfo = json::GetAtlasFromFile(this->mEntrySettingPath);
    MDY_ASSERT_MSG_FORCE(opMetaInfo.has_value() == true, "Failed to open meta data file.");
    const auto& metaAtlas = opMetaInfo.value();

    // Apply setting to project before everthing starts to working.
    const auto& settingAtlas = metaAtlas["Setting"];
    json::GetValueFromTo(settingAtlas, sCategoryDescription, this->mDescription);
    json::GetValueFromTo(settingAtlas, sCategoryGameplay, this->mGamePlay);
    json::GetValueFromTo(settingAtlas, sCategoryInput, this->mInput);
    json::GetValueFromTo(settingAtlas, sCategoryTag, this->mTag);
    json::GetValueFromTo(settingAtlas, "Sound", this->mSound);
    json::GetValueFromTo(settingAtlas, "Physics", this->mPhysics);
    MIOMeta::GetInstance().MDY_PRIVATE(InitiateMetaInformationComp)(metaAtlas);
  }
  else { MDY_UNEXPECTED_BRANCH(); }

  this->mIsInitialized = true;
  return DY_SUCCESS;
}

EDySuccess MSetting::pfRelease()
{
  DyPushLogDebugInfo("{} | MSetting::pfRelease()", "Function call");
  return DY_SUCCESS;
}

} /// ::dy namespace