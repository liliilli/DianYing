#include <precompiled.h>
#if defined(MDY_FLAG_IN_EDITOR) && defined(MDY_PLATFORM_FLAG_WINDOWS)
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
#include <Dy/Editor/Helper/Win32Helper.h>
#include <ShObjIdl_core.h>

//!
//! Local functions
//!

namespace
{



} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy::editor
{

std::wstring DyWinGetDirectoryPathFromDialog() noexcept
{
  HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
  if (!SUCCEEDED(hr)) { return L""; }

  IFileOpenDialog *pFileOpen;

  // Create the FileOpenDialog object.
  hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));
  if (!SUCCEEDED(hr)) { CoUninitialize(); return L""; }

  // Show the Open dialog box.
  pFileOpen->SetOptions(FOS_PICKFOLDERS);
  hr =  pFileOpen->Show(nullptr);

  // Get the file name from the dialog box.
  if (!SUCCEEDED(hr)) { return L""; }

  IShellItem *pItem = nullptr;
  hr = pFileOpen->GetResult(&pItem);
  if (!SUCCEEDED(hr)) { return L""; }

  PWSTR pszFilePath = nullptr;
  hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

  // Display the file name to the user.
  std::wstring wideString;
  if (SUCCEEDED(hr))
  {
    wideString = std::wstring(pszFilePath);
    CoTaskMemFree(pszFilePath);
  }

  pItem->Release();
  pFileOpen->Release();
  CoUninitialize();

  return wideString;
}

std::wstring DyWinGetFilePathFromDialog() noexcept
{
  HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
  if (!SUCCEEDED(hr)) { return L""; }

  IFileOpenDialog *pFileOpen;

  // Create the FileOpenDialog object.
  hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));
  if (!SUCCEEDED(hr)) { CoUninitialize(); return L""; }

  // Show the Open dialog box.
  hr =  pFileOpen->Show(nullptr);

  // Get the file name from the dialog box.
  if (!SUCCEEDED(hr)) { return L""; }

  IShellItem *pItem = nullptr;
  hr = pFileOpen->GetResult(&pItem);
  if (!SUCCEEDED(hr)) { return L""; }

  PWSTR pszFilePath = nullptr;
  hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

  // Display the file name to the user.
  std::wstring wideString;
  if (SUCCEEDED(hr))
  {
    wideString = std::wstring(pszFilePath);
    CoTaskMemFree(pszFilePath);
  }

  pItem->Release();
  pFileOpen->Release();
  CoUninitialize();

  return wideString;
}

} /// ::dy::editor namespace

#endif /// MDY_FLAG_IN_EDITOR && MDY_PLATFORM_FLAG_WINDOWS