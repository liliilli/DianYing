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

#include <cassert>

#include <iostream>

#ifdef false
#include <Dy/VkInterface.h>
#endif

#include <d3dx11effect.h>

#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>

#include <Dy/Management/DataInformationManager.h>
#include <Dy/Management/HeapResourceManager.h>
#include <Dy/Management/SceneManager.h>
#include <Dy/Management/SettingManager.h>
#include <Dy/Management/TimeManager.h>
#include <Dy/Management/WindowManager.h>
#include <Dy/Management/LoggingManager.h>
#include "Dy/Management/InputManager.h"

/// @todo Chapter 6
struct DVertex1
{
  DirectX::XMFLOAT3 position;
  DirectX::XMFLOAT4 color;
};

struct DVectex2
{
  DirectX::XMFLOAT3 position;
  DirectX::XMFLOAT2 normal;
  DirectX::XMFLOAT2 texCoord0;
  DirectX::XMFLOAT2 texCoord1;
};

//!
//! DirectX12 API
//!

void                  DyD12RenderLoop();
void                  DyD12RenderFrame();

ID3D11Device*         d11Device         = nullptr;
ID3D11DeviceContext*  d11DeviceContext  = nullptr;

//!
//! OpenGL 4 API
//!

EDySuccess                DyGlCreateContentWgl();

namespace
{
///
/// @brief Initialize all managers related to DianYing rendering application.
/// Initialzation order must be ended with MDyWindow and started with MDySetting by getting argv
/// as option.
///
void DyInitiailzeAllManagers()
{
  { // Setting manager
    auto& settingManager = dy::MDySetting::GetInstance();
    #if defined(MDY_PLATFORM_FLAG_WINDOWS) && defined(_WIN32)
      const int32_t size = __argc;
      for (int32_t i = 0; i < size; ++i) {
        settingManager.ArgsPushback(__argv[i]);
      }
    #elif defined(MDY_PLATFORM_FLAG_LINUX) && defined(__linux__)

    #elif defined(MDY_PLATFORM_FLAG_MACOS)

    #endif
    MDY_CALL_ASSERT_SUCCESS(dy::MDySetting::Initialize());
  }

  auto& i = dy::MDySetting::GetInstance();
  i.SetSubFeatureLoggingToConsole(true);
  i.SetSubFeatureLoggingToFile(true);
  MDY_CALL_ASSERT_SUCCESS(dy::MDyLog::Initialize());

  MDY_CALL_ASSERT_SUCCESS(dy::MDyTime::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyDataInformation::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyResource::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyScene::Initialize());

  // MDyWindow must be initialized at last.
  MDY_CALL_ASSERT_SUCCESS(dy::MDyWindow::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyInput::Initialize());
}

///
/// @brief Release all managers related to DianYing rendering application.
/// Order must be revered with DyInitializeAllManagers().
///
void DyReleaseAllManagers()
{
  MDY_CALL_ASSERT_SUCCESS(dy::MDyInput::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyWindow::Release());

  // Release other management instance.
  MDY_CALL_ASSERT_SUCCESS(dy::MDyScene::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyResource::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyDataInformation::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyTime::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyLog::Release());

  MDY_CALL_ASSERT_SUCCESS(dy::MDySetting::Release());
}

} /// unnamed namespace

//!
//! TBB TEST START
//!

class DyTestSubStringFinder
{
  const std::string     mString;
  std::vector<int32_t>* mMaxArr = nullptr;
  std::vector<int32_t>* mPosArr = nullptr;

public:
  DyTestSubStringFinder(
      const std::string& str,
      std::vector<int32_t>& maxArr,
      std::vector<int32_t>& posArr) :
      mString{str}, mMaxArr(&maxArr), mPosArr(&posArr)
  { }

  void operator() (const tbb::blocked_range<int32_t>& r) const
  {
    for (int32_t i = r.begin(); i != r.end(); ++i)
    {
      int32_t maxSize = 0, maxPos = 0;
      for (int32_t j = 0, strSize = static_cast<int32_t>(mString.size()); j < strSize; ++j)
      {
        if (j != i)
        {
          const int32_t limit = strSize - std::max(i, j);
          for (int32_t k = 0; k < limit; ++k)
          {
            if (this->mString[i + k] != this->mString[j + k]) break;
            if (k > maxSize)
            {
              maxSize = k;
              maxPos  = j;
            }
          }
        }

        this->mMaxArr->operator[](i) = maxSize;
        this->mPosArr->operator[](i) = maxPos;
      }
    }
  }
};

//!
//! TBB TEST END
//!

///
/// @brief Main entry function of WIN32 platforms.
///
#if defined(MDY_PLATFORM_FLAG_WINDOWS)
namespace
{

FILE* fp = nullptr;

///
/// @brief Turn on memory leak detection feature and console window for logging.
///
EDySuccess __InitializeWin32Debug()
{
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
  _CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG );

  if (!AllocConsole()) {
    MessageBox(nullptr, L"The console window was not created.", nullptr, MB_ICONEXCLAMATION);
    return DY_FAILURE;
  }

  freopen_s(&fp, "CONOUT$", "w", stdout);
  std::cout << "Hello world!\n";
  return DY_SUCCESS;
}

///
/// @brief Turn off memory leak detection feature and console window for logging.
///
EDySuccess __ReleaseWin32Debug()
{
  fclose(fp);

  if (!FreeConsole()) {
    MessageBox(nullptr, L"Failed to free console resource.", nullptr, MB_ICONEXCLAMATION);
    return DY_FAILURE;
  }

  return DY_SUCCESS;
}

} /// unname namespace

#if defined(_DEBUG) || !defined(NDEBUG)
#define MDY_WIN32_TRY_TURN_ON_DEBUG()   MDY_CALL_ASSERT_SUCCESS(__InitializeWin32Debug())
#define MDY_WIN32_TRY_TURN_OFF_DEBUG()  MDY_CALL_ASSERT_SUCCESS(__ReleaseWin32Debug())
#else
#define MDY_WIN32_TRY_TURN_ON_DEBUG()   (void)0
#define MDY_WIN32_TRY_TURN_OFF_DEBUG()  (void)0
#endif

///
/// @brief Main function of win32 / win64 platform.
///
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) {
  MDY_WIN32_TRY_TURN_ON_DEBUG();
  DyInitiailzeAllManagers();

  MDY_LOG_INFO_D("Platform : Windows");
  MDY_LOG_INFO_D("Running application routine.");

#ifdef false
  std::vector<std::string> str = { std::string("a"), std::string("b") };
  str.resize(18);
  for (int32_t i = 2; i < str.size(); ++i)
  {
    str[i] = str[i - 1] + str[i - 2];
  }
  std::string& toScan = *str.rbegin();
  const int32_t numElem = static_cast<int32_t>(toScan.size());

  std::vector<int32_t> max(numElem);
  std::vector<int32_t> pos(numElem);

  tbb::parallel_for(tbb::blocked_range<int32_t>(0, numElem),
                    DyTestSubStringFinder(toScan, max, pos));

  for (int32_t i = 0; i < numElem; ++i)
  {
    MDY_LOG_CRITICAL_D("{} ( {} )", max[i], pos[i]);
  }
#endif

  dy::MDyWindow::GetInstance().Run();

  MDY_LOG_INFO_D("Release all managers and resources.");

  DyReleaseAllManagers();
  MDY_WIN32_TRY_TURN_OFF_DEBUG();
  return 0;
}
#elif defined(MDY_PLATFORM_FLAG_LINUX)
int main()
{

  return 0;
}
#elif defined(MDY_PLATFORM_FLAG_MACOS)
int main()
{

  return 0;
}
#endif