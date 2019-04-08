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

#include <FWindowsPlatform.h>
#include <sstream>

#include <EPlatform.h>
#include <FWindowsHandles.h>
#include <FBtResourceHandle.h>

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include <GLFW/glfw3native.h>
#include <atlconv.h>

namespace dy
{

FWindowsPlatform::FWindowsPlatform() 
  : APlatformBase{EPlatform::Windows}
{
  this->mHandle = std::make_unique<FWindowsHandles>(GetCurrentProcess());
}

FWindowsPlatform::~FWindowsPlatform()
{

}

void FWindowsPlatform::SetWindowTitle(const std::string& newTitle)
{
  auto& handle = static_cast<FWindowsHandles&>(*this->mHandle);
  if (handle.mGlfwWindow == nullptr) { return; }

  glfwSetWindowTitle(handle.mGlfwWindow, newTitle.c_str());

#if 0
  // Widen string.
  std::wstring wideStr;
  const auto& ctfacet = std::use_facet<std::ctype<wchar_t>>(std::wstringstream().getloc());

  for (const auto& currChar : newTitle)
  {
    wideStr.push_back(ctfacet.widen(currChar));
  }

  // Try set window text.
  const auto isSucceeded = SetWindowText(handle.mMainWindow, wideStr.c_str());
  if (isSucceeded == FALSE)
  {
    const auto errorFlag = GetLastError();
  }
#endif
}

std::string FWindowsPlatform::GetWindowTitle() const
{
  auto& handle = static_cast<FWindowsHandles&>(*this->mHandle);

  // Get title name from main window handle.
  std::wstring titleName = {256, L'0'};
  GetWindowText(handle.mMainWindow, titleName.data(), static_cast<int>(titleName.size()));

  // If failed to get window text, just return empty string.
  if (titleName.length() == 0)
  {
    const auto errorFlag = GetLastError();
    return "";
  }

  // Narrow string into std::string (<char>)
  std::string resultTitleName;
  const auto& ctfacet = std::use_facet<std::ctype<wchar_t>>(std::wstringstream().getloc());

  for (const auto& currWChar : titleName)
  {
    if (currWChar == L'0') { continue; }
    resultTitleName.push_back( ctfacet.narrow(currWChar, 0 ) );
  }
       
  // Return.
	return resultTitleName;
}

uint32_t FWindowsPlatform::GetWindowHeight() const
{
  auto& handle = static_cast<FWindowsHandles&>(*this->mHandle);
  if (handle.mGlfwWindow == nullptr) { return 0; }

  int width, height;
  glfwGetWindowSize(handle.mGlfwWindow, &width, &height);

  return height;

#if 0
  // Get RECT from main window.
  RECT rect;
  const auto isSucceeded = GetWindowRect(handle.mMainWindow, &rect);
  if (isSucceeded == FALSE)
  {
    const auto errorFlag = GetLastError();
    return 0;
  }

  // Return values.
  return rect.bottom - rect.top;
#endif
}

uint32_t FWindowsPlatform::GetWindowWidth() const
{
  auto& handle = static_cast<FWindowsHandles&>(*this->mHandle);
  if (handle.mGlfwWindow == nullptr) { return 0; }

  int width, height;
  glfwGetWindowSize(handle.mGlfwWindow, &width, &height);

  return width;

#if 0
  // Get RECT from main window.
  RECT rect;
  const auto isSucceeded = GetWindowRect(handle.mMainWindow, &rect);
  if (isSucceeded == FALSE)
  {
    const auto errorFlag = GetLastError();
    return 0;
  }

  // Return values.
  return rect.right - rect.left;
#endif
}

void FWindowsPlatform::ResizeWindow(uint32_t width, uint32_t height)
{
  auto& handle = static_cast<FWindowsHandles&>(*this->mHandle);
  if (handle.mGlfwWindow == nullptr) { return; }

  // Set size.
  glfwSetWindowSize(handle.mGlfwWindow, width, height);
}

#undef FindResource

std::unique_ptr<ABtResourceBase> 
FWindowsPlatform::FindResource(int id, EXPR_E(EBtResource) type)
{
#define FindResource FindResourceW
  USES_CONVERSION;
  HRSRC hResource = FindResource(
    nullptr, 
    MAKEINTRESOURCE(id), 
    A2W(EBtResource::ToString(type))
  );
  if (hResource == nullptr) { return nullptr; }

  return std::make_unique<FBtResourceHandle>(hResource);
}


} /// ::dy namespace