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

#include <cstdio>
#include <array>

#include <FWindowsPlatform.h>
#include <PLowInputMousePos.h>
#include <cassert>
#include <iostream>
#include <glad/glad.h>

#ifdef _WIN32
  #pragma comment(lib, "opengl32.lib")
#endif

std::unique_ptr<dy::APlatformBase> platform = nullptr;

int WINAPI WinMain(
  [[maybe_unused]] HINSTANCE hInstance, 
  [[maybe_unused]] HINSTANCE hPrevInstance, 
  [[maybe_unused]] LPSTR lpCmdLine, 
  [[maybe_unused]] int nCmdShow)
{
  platform = std::make_unique<dy::FWindowsPlatform>();
  platform->InitPlatform();
  platform->CreateConsoleWindow();

#ifdef _WIN32
  #ifdef CreateWindow
    #undef CreateWindow
  #endif
#endif

  dy::PWindowCreationDescriptor desc = {};
  desc.mIsWindowFullScreen = false;
  desc.mIsWindowResizable = true;
  desc.mIsWindowShouldFocus = true;
  desc.mIsWindowVisible = true;
  desc.mWindowName = "GL Context Test";
  desc.mWindowWidth = 800;
  desc.mWindowHeight = 600;

  auto optRes = platform->CreateWindow(desc);
  assert(optRes.has_value() == true);

  auto& input = platform->GetInputManager();
  input.SetMousePosFeatureState(dy::base::ELowMousePosState::Normal);

  // Init Context 
  PIXELFORMATDESCRIPTOR pfd; // Win32 Pixel format of drawing surface.
  HGLRC prevRc, rc;
  HDC prevDc, dc;

  auto instance = LoadLibraryA("opengl32.dll");
  if (instance == nullptr)
  {
    std::cerr << "Failed to load opengl module.\n";
    return 3;
  }

  const auto ptrHandle = static_cast<HWND>(platform->_GetHandleOf(*optRes));
  dc = GetDC(ptrHandle);

  // Set pixel format descriptor.
  ZeroMemory(&pfd, sizeof(pfd));
  pfd.nSize = sizeof(pfd);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 24;

  // Get pixel format and save into pfd.
  const auto pf = ChoosePixelFormat(dc, &pfd);
  if (DescribePixelFormat(dc, pf, sizeof(pfd), &pfd) == FALSE)
  {
    return 3;
  }

  // Set pixel format into dc.
  if (SetPixelFormat(dc, pf, &pfd) == FALSE)
  {
    platform->GetDebugManager().OnAssertionFailed(
      "Failed to set pixel format for dummy context.",
      __FUNCTION__, __FILE__, __LINE__
    );
    return 3;
  }

  // Create GL context. 
  rc = wglCreateContext(dc);
  if (rc == nullptr)
  {
    platform->GetDebugManager().OnAssertionFailed(
      "Failed to set create dummy context.",
      __FUNCTION__, __FILE__, __LINE__
    );
    return 3;
  }
  prevDc = wglGetCurrentDC();
  prevRc = wglGetCurrentContext();

  if (wglMakeCurrent(dc, rc) == FALSE)
  {
    platform->GetDebugManager().OnAssertionFailed(
      "Failed to make dummy context current.",
      __FUNCTION__, __FILE__, __LINE__
    );

    wglMakeCurrent(prevDc, prevRc);
    wglDeleteContext(rc);

    return 3;
  }

  if (!gladLoadGL())
  {
    return 3;
  }

  // Init GL
  glViewport(0, 0, 800, 600);
  glClearColor(0.0f, 0.0f, 0.0f, 0.f);

	while (platform->CanShutdown() == false)
	{
    platform->PollEvents();

    // Render 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glBegin(GL_TRIANGLES);
    {
      glVertex2f(0.0, 0.5);
      glVertex2f(-0.5, -0.5);
      glVertex2f(0.5, -0.5);
    }
    glEnd();
    glFlush();

    SwapBuffers(dc);
	}

  // Remove GL

  // Remove Context 
  wglMakeCurrent(prevDc, prevRc);
  wglDeleteContext(rc);
  rc = nullptr;

  ReleaseDC(ptrHandle, dc);
  dc = nullptr;

  FreeLibrary(instance);
  instance = nullptr;

  platform->RemoveAllWindow();
  platform->RemoveConsoleWindow();
  platform->ReleasePlatform();
	return 0;
}
