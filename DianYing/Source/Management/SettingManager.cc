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

#include <iostream>
#include <cassert>

#include <Dy/Management/LoggingManager.h>

namespace
{

[[nodiscard]]
bool __IsSameCString(const char* lhs, const char* rhs) noexcept
{
  return strcmp(lhs, rhs) == 0;
}

[[nodiscard]]
dy::ERenderingType __GetRenderingType(const std::vector<const char*>& argsList) noexcept
{
  /// Temporal structure to bind option string and type.
  struct DItem final {
    const char*         mCString = "";
    dy::ERenderingType  mType = dy::ERenderingType::None;

    explicit DItem(const char* cString, dy::ERenderingType type) : mCString(cString), mType(type) {};
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
    DItem{"vulkan", dy::ERenderingType::Vulkan},
    DItem{"opengl", dy::ERenderingType::OpenGL},
    DItem{"d3d11", dy::ERenderingType::DirectX11},
    DItem{"d3d12", dy::ERenderingType::DirectX12}
  };

  for (const auto& item : renderingTypeList)
  {
    if (__IsSameCString(optionString, item.mCString))
    {
      return item.mType;
    }
  }

  return dy::ERenderingType::None;
}

} /// unnamed namespace

namespace dy
{

ERenderingType MDySetting::GetRenderingType() const noexcept
{
  assert(this->mIsInitialized);
  return this->mRenderingType;
}

void MDySetting::SetFeatureLogging(bool isEnabled) noexcept
{
  if (this->mIsEnabledLogging != isEnabled)
  {
    auto& logManager = dy::MDyLog::GetInstance();
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
}

void MDySetting::SetSubFeatureLoggingToFile(bool isEnabled) noexcept
{
  this->mIsEnabledLoggingToFile = isEnabled;
}

void MDySetting::ArgsPushback(const char* argsString)
{
  assert(!this->mIsInitialized);
  this->mApplicationArgs.emplace_back(argsString);
}

bool MDySetting::IsVSyncEnabled() const noexcept
{
  assert(this->mIsInitialized);
  return this->mIsEnabledVsync;
}

void MDySetting::SetVSyncMode(bool enableVsync) noexcept
{

}

EDySuccess MDySetting::pfInitialize()
{
  std::cout << "MDySetting::pfInitialize()\n";
  if (const auto type = __GetRenderingType(mApplicationArgs);
      type == ERenderingType::None) {
    return DY_FAILURE;
  }
  else {
    this->mRenderingType = type;
  }

  mIsInitialized = true;
  return DY_SUCCESS;
}

EDySuccess MDySetting::pfRelease()
{
  std::cout << "MDySetting::pfRelease()\n";
  return DY_SUCCESS;
}

} /// ::dy namespace