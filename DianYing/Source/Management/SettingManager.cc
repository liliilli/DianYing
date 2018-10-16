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

#include <Dy/Management/LoggingManager.h>
#include <Dy/Management/TimeManager.h>
#include <Dy/Helper/JsonHelper.h>

namespace
{

//!
//! Local translation unit varaibles
//!

MDY_SET_IMMUTABLE_STRING(sSettingPathName, "./TestSetting.DDat");

MDY_SET_IMMUTABLE_STRING(sCategoryDescription,  "Description");
MDY_SET_IMMUTABLE_STRING(sCategoryGameplay,     "Gameplay");
MDY_SET_IMMUTABLE_STRING(sCategoryInput,        "Input");

MDY_SET_IMMUTABLE_STRING(sProjectName,          "ProjectName");
MDY_SET_IMMUTABLE_STRING(sWindowName,           "WindowName");
MDY_SET_IMMUTABLE_STRING(sVersionHigh,          "VersionHigh");
MDY_SET_IMMUTABLE_STRING(sVersionMid,           "VersionMid");
MDY_SET_IMMUTABLE_STRING(sVersionLow,           "VersionLow");
MDY_SET_IMMUTABLE_STRING(sCompanyName,          "CompanyName");
MDY_SET_IMMUTABLE_STRING(sHomepage,             "Homepage");
MDY_SET_IMMUTABLE_STRING(sSupportContact,       "SupportContact");

MDY_SET_IMMUTABLE_STRING(sInitialScene,         "InitialScene");
MDY_SET_IMMUTABLE_STRING(sInitWidth,            "InitWidth");
MDY_SET_IMMUTABLE_STRING(sInitHeight,           "InitHeight");

//!
//! Global function
//!

///
/// @brief
///
[[nodiscard]] bool __IsSameCString(const char* lhs, const char* rhs) noexcept
{
  return strcmp(lhs, rhs) == 0;
}

///
/// @brief
///
[[nodiscard]] dy::EDyRenderingApiType __GetRenderingType(const std::vector<const char*>& argsList) noexcept
{
  /// Temporal structure to bind option string and type.
  struct DItem final
  {
    const char*         mCString = "";
    dy::EDyRenderingApiType  mType = dy::EDyRenderingApiType::NoneError;

    explicit DItem(const char* cString, dy::EDyRenderingApiType type) : mCString(cString), mType(type) {};
  };

#if defined(_WIN32)
  assert(argsList.size() == 2);
  const char* optionString = argsList[1];
#elif defined(__linux__)
  assert(argsList.size() == 2);
  const char* optionString = argsList[1];
#endif

  const std::vector<DItem> renderingTypeList =
  {
    DItem{"vulkan", dy::EDyRenderingApiType::Vulkan},
    DItem{"opengl", dy::EDyRenderingApiType::OpenGL},
    DItem{"d3d11", dy::EDyRenderingApiType::DirectX11},
    DItem{"d3d12", dy::EDyRenderingApiType::DirectX12}
  };

  for (const auto& item : renderingTypeList)
  {
    if (__IsSameCString(optionString, item.mCString))
    {
      return item.mType;
    }
  }

  return dy::EDyRenderingApiType::NoneError;
}

} /// unnamed namespace

//!
//! Implementation
//!

namespace dy
{

EDyRenderingApiType MDySetting::GetRenderingType() const noexcept
{
  assert(this->mIsInitialized);
  return this->mRenderingType;
}

void MDySetting::SetFeatureLogging(bool isEnabled) noexcept
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

void MDySetting::SetSubFeatureLoggingToConsole(bool isEnabled) noexcept
{
  this->mIsEnabledLoggingToConsole = isEnabled;
  MDY_LOG_INFO_D("{} | Logging Console : {}. Need to be restart logger.", "SubFeature", isEnabled ? "ON" : "OFF");
}

void MDySetting::SetSubFeatureLoggingToFile(bool isEnabled) noexcept
{
  this->mIsEnabledLoggingToFile = isEnabled;
  MDY_LOG_INFO_D("{} | Logging File : {}. Need to be restart logger.", "SubFeature", isEnabled ? "ON" : "OFF");
}

void MDySetting::SetLogFilePath(const std::string& path) noexcept
{
  if (path.empty())
  {
    MDY_LOG_ERROR_D("{} | new log file path is empty. Log file path did not change. Log file path : {}", this->mLogFilePath);
  }
  else
  {
    this->mLogFilePath = path;
    MDY_LOG_INFO_D("{} | Update Log file path : {}. Need to be restart logger.", "SubFeature", this->mLogFilePath);
  }
}

void MDySetting::pArgsPushback(const char* argsString)
{
  MDY_ASSERT(!this->mIsInitialized, "Setting manager must not be initiailized before putting arguments");
  this->mApplicationArgs.emplace_back(argsString);
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

EDySuccess MDySetting::pfInitialize()
{
  ///
  /// @function InitializeGraphicsApi
  /// @brief Initialize graphics api.
  ///
  static auto InitializeGraphicsApi = [](MDySetting& manager) -> EDySuccess
  {
    for (const auto& args : manager.mApplicationArgs) { MDY_LOG_INFO_D("{} | Arguments : {}", "Feature", args); }
    MDY_LOG_INFO_D("{} | Logging : {}", "Feature", manager.mIsEnabledLogging ? "ON" : "OFF");
    MDY_LOG_INFO_D("{} | Logging Console : {}", "SubFeature", manager.mIsEnabledLoggingToConsole ? "ON" : "OFF");
    MDY_LOG_INFO_D("{} | Logging File : {}", "SubFeature", manager.mIsEnabledLoggingToFile ? "ON" : "OFF");
    MDY_LOG_INFO_D("{} | Logging File path : {}", "SubFeature", manager.mLogFilePath);

    // Set rendering api type.
    if (const auto type = __GetRenderingType(manager.mApplicationArgs); type == EDyRenderingApiType::NoneError)
    {
      return DY_FAILURE;
    }
    else {
      manager.mRenderingType = type;
    }

    MDY_LOG_INFO_D("{} | Vsync : {}", "Feature", manager.mIsEnabledVsync ? "ON" : "OFF");
    switch (manager.mRenderingType)
    {
    case EDyRenderingApiType::Vulkan:     MDY_LOG_INFO_D("{} | Graphics API : {}", "Feature", "Vulkan");    break;
    case EDyRenderingApiType::DirectX11:  MDY_LOG_INFO_D("{} | Graphics API : {}", "Feature", "DirectX11"); break;
    case EDyRenderingApiType::DirectX12:  MDY_LOG_INFO_D("{} | Graphics API : {}", "Feature", "DirectX12"); break;
    case EDyRenderingApiType::OpenGL:     MDY_LOG_INFO_D("{} | Graphics API : {}", "Feature", "OpenGL");    break;
    default:                              MDY_LOG_INFO_D("{} | Graphics API : {}", "Feature", "Unknown");   break;
    }

    return DY_SUCCESS;
  };

  // Output setting options at debug mode.
  MDY_LOG_INFO_D("{} | MDySetting::pfInitialize().", "FunctionCall");

  if (InitializeGraphicsApi(*this) == DY_FAILURE)
  {
    return DY_FAILURE;
  }

  if (const auto opSettingAtlas = DyGetJsonAtlas(sSettingPathName.data());
      !opSettingAtlas.has_value())
  {
    return DY_FAILURE;
  }
  else
  { // Apply setting to project before everthing starts to working.
    const auto& settingAtlas = opSettingAtlas.value();
    { // Category[Description]
      const auto& description = settingAtlas.at(sCategoryDescription.data());

      this->mProjectName  = description.at(sProjectName.data()) .get<std::string>();
      this->mWindowName   = description.at(sWindowName.data())  .get<std::string>();
      this->mCompanyName  = description.at(sCompanyName.data()) .get<std::string>();
      this->mHomepage     = description.at(sHomepage.data())    .get<std::string>();
      this->mSupportContact = description.at(sSupportContact.data()).get<std::string>();

      this->mVersionHigh  = description.at(sVersionHigh.data()) .get<TI32>();
      this->mVersionMid   = description.at(sVersionMid.data())  .get<TI32>();
      this->mVersionLow   = description.at(sVersionLow.data())  .get<TI32>();
    }

    { // Category[Gameplay]
      const auto& gameplay = settingAtlas.at(sCategoryGameplay.data());

      this->mInitialSceneName = gameplay.at(sInitialScene.data()) .get<std::string>();
      this->mWindowSizeWidth  = gameplay.at(sInitWidth.data())    .get<TI32>();
      this->mWindowSizeHeight = gameplay.at(sInitHeight.data())   .get<TI32>();
    }

    { // Input[Gameplay]
      [[maybe_unused]]
      const auto& input = settingAtlas.at(sCategoryInput.data());
    }
  }

  mIsInitialized = true;
  return DY_SUCCESS;
}

EDySuccess MDySetting::pfRelease()
{
  MDY_LOG_INFO_D("{} | MDySetting::pfRelease()", "Function call");
  return DY_SUCCESS;
}

} /// ::dy namespace