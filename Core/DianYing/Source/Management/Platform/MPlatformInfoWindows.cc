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
#include <Dy/Management/Platform/MPlatformInfoWindows.h>
#include <sstream>

#if (defined(_WIN32) == true)

#include <Psapi.h>
#include <strsafe.h>
#include <atlconv.h>

//!
//! Local translation unit function.
//!

namespace
{

/// @brief Callback function of `bool MPlatformInfoWindows::IsFontExistOnSystem(_MIN_ const std::string& iFontKey)`
int CALLBACK EnumFontFamExProc(
    _MIN_ const LOGFONT* lpelfe, 
    _MIN_ const TEXTMETRIC* lpntme, 
    _MIN_ DWORD FontType, 
    _MIN_ LPARAM lparam)
{
  std::wstring* ptrTargetFaceName = reinterpret_cast<std::wstring*>(lparam); 
  if (*ptrTargetFaceName == lpelfe->lfFaceName)
  {
    return 0;
  }
  else
  {
    return 1;
  }
}

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy
{

void MPlatformInfoWindows::InitializeDep()
{
  SYSTEM_INFO sysInfo;
  FILETIME ftime, fsys, fuser;

  GetSystemInfo(&sysInfo);
  mNumProcessors = sysInfo.dwNumberOfProcessors;

  GetSystemTimeAsFileTime(&ftime);
  memcpy(&mLastCpu, &ftime, sizeof(FILETIME));

  mSelf = GetCurrentProcess();
  GetProcessTimes(mSelf, &ftime, &ftime, &fsys, &fuser);
  memcpy(&mLastSysCpu, &fsys, sizeof(FILETIME));
  memcpy(&mLastUserCpu, &fuser, sizeof(FILETIME));
}

void MPlatformInfoWindows::ReleaseDep()
{ 

}

EDySuccess MPlatformInfoWindows::CreateConsoleWindow()
{
  MDY_ASSERT_MSG(
    this->mIsConsoleWindowInitialized.load() == false, 
    "MWindow::mIsConsoleWindowInitialized must be falsed.");

  // Allocate console and forward stdout to console.
  if (AllocConsole() == false)
  {
    MessageBox(nullptr, L"The console window was not created.", nullptr, MB_ICONEXCLAMATION);
    return EDySuccess::DY_FAILURE;
  }
  [[maybe_unused]] const auto _ = freopen_s(&this->mFp, "CONOUT$", "w", stdout);

  bool before = false;
  while (this->mIsConsoleWindowInitialized.compare_exchange_strong(
    before, true, std::memory_order_seq_cst) == false)
      ;
  return EDySuccess::DY_SUCCESS;
}

bool MPlatformInfoWindows::IsCreatedConsoleWindow() const noexcept
{
  return this->mIsConsoleWindowInitialized.load();
}

EDySuccess MPlatformInfoWindows::RemoveConsoleWindow()
{
  MDY_ASSERT_MSG(this->mIsConsoleWindowInitialized.load() == true, "MWindow::mIsConsoleWindowInitialized must be trued.");

  // Release console.
  fclose(this->mFp);
  if (FreeConsole() == false)
  {
    MessageBox(nullptr, L"Failed to free console resource.", nullptr, MB_ICONEXCLAMATION);
    return EDySuccess::DY_FAILURE;
  }

  bool before = true;
  while (this->mIsConsoleWindowInitialized.compare_exchange_strong(before, false, std::memory_order_seq_cst) == false)
      ;
  return EDySuccess::DY_SUCCESS;
}

TF32 MPlatformInfoWindows::GetCpuUsage()
{
  FILETIME ftime, fsys, fuser;
  ULARGE_INTEGER now, sys, user;

  GetSystemTimeAsFileTime(&ftime);
  memcpy(&now, &ftime, sizeof(FILETIME));

  GetProcessTimes(mSelf, &ftime, &ftime, &fsys, &fuser);
  memcpy(&sys, &fsys, sizeof(FILETIME));
  memcpy(&user, &fuser, sizeof(FILETIME));
  TF32 percent = (sys.QuadPart - mLastSysCpu.QuadPart) + (user.QuadPart - mLastUserCpu.QuadPart);
  percent /= (now.QuadPart - mLastCpu.QuadPart);
  percent /= mNumProcessors;
  mLastCpu     = now;
  mLastUserCpu = user;
  mLastSysCpu  = sys;

  return percent * 100;
}

TU64 MPlatformInfoWindows::GetRamUsage()
{
  PROCESS_MEMORY_COUNTERS_EX pmc;
  GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
  return pmc.WorkingSetSize;
}

/// @reference https://stackoverflow.com/questions/2702157/get-list-of-fonts-win32
/// @reference https://docs.microsoft.com/en-us/previous-versions//dd162618(v=vs.85)
/// @reference https://docs.microsoft.com/en-us/windows/desktop/api/Wingdi/nf-wingdi-enumfontfamiliesexa
bool MPlatformInfoWindows::IsFontExistOnSystem(_MIN_ const std::string& iFontKey) const
{
  HDC pDC = GetDC(NULL);
  LOGFONT logFont;

  // Set font information structure.
  logFont.lfCharSet = DEFAULT_CHARSET;
  logFont.lfPitchAndFamily = 0;

  // @reference https://www.gpgstudy.com/forum/viewtopic.php?t=7047
  std::wstring fontKey;
  {
    USES_CONVERSION;
    fontKey = A2W(iFontKey.c_str());
  }
  wcscpy(logFont.lfFaceName, fontKey.c_str());

  // EnumFontFamiliesEXW (Multi-byte dirty function) returns `0` when found given font face-name.
  const auto flag = EnumFontFamiliesExW(pDC, &logFont, EnumFontFamExProc, reinterpret_cast<LPARAM>(&fontKey), 0);
  return flag == 0;
}

std::optional<std::string> MPlatformInfoWindows::GetFontPathOnSystem(_MIN_ const std::string& iFontKey) const
{
  // Check validity.
  if (this->IsFontExistOnSystem(iFontKey) == false) { return std::nullopt; }
  
  // @reference https://www.gpgstudy.com/forum/viewtopic.php?t=7047
  std::wstring fontFacename;
  {
    USES_CONVERSION;
    fontFacename = A2W(iFontKey.c_str());
  }

  // Find path using registry.
  static const auto fontRegistryPath = L"Software\\Microsoft\\Windows NT\\CurrentVersion\\Fonts";
  HKEY hKey;

  // Open Windows font registry key
  {
    const auto result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, fontRegistryPath, 0, KEY_READ, &hKey);
    if (result != ERROR_SUCCESS) { return std::nullopt; }
  }

  DWORD maxValueNameSize, maxValueDataSize;
  {
    const auto result = RegQueryInfoKey(hKey, 0, 0, 0, 0, 0, 0, 0, &maxValueNameSize, &maxValueDataSize, 0, 0);
    if (result != ERROR_SUCCESS) { return std::nullopt; }
  }

  DWORD valueIndex = 0;
  LPWSTR valueName = new WCHAR[maxValueNameSize];
  LPBYTE valueData = new BYTE[maxValueDataSize];
  DWORD valueNameSize, valueDataSize, valueType;
  std::wstring wsFontFile;

  { // Look for a matching font name
    LONG result;
    do 
    {
      wsFontFile.clear();
      valueDataSize = maxValueDataSize;
      valueNameSize = maxValueNameSize;

      result = RegEnumValue(hKey, valueIndex, valueName, &valueNameSize, 0, &valueType, valueData, &valueDataSize);
      valueIndex++;

      if (result != ERROR_SUCCESS || valueType != REG_SZ) { continue; }

      std::wstring wsValueName(valueName, valueNameSize);

      // Find a match. 
      if (wsValueName.find(fontFacename) != std::wstring::npos) { wsFontFile.assign((LPWSTR)valueData, valueDataSize); break; }
    }
    while (result != ERROR_NO_MORE_ITEMS);
  }

  // Release temporary chunk.
  delete[] valueName; valueName = nullptr; delete[] valueData; valueData = nullptr;

  // Close registry.
  RegCloseKey(hKey);
  if (wsFontFile.empty()) { return std::nullopt; }

  // Build full font file path
  WCHAR winDir[MAX_PATH];
  GetWindowsDirectory(winDir, MAX_PATH);

  std::wstringstream ss; ss << winDir << "\\Fonts\\" << wsFontFile;
  wsFontFile = ss.str();

  return std::string(wsFontFile.begin(), wsFontFile.end());
}

} /// ::dy namespace

#endif /// MDY_PLATFORM_FLAG_WINDOWS