#ifndef DIANYING_PRECOMPILED_H
#define DIANYING_PRECOMPILED_H
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

/// C/C++ Header files

#include <cassert>
#include <cstdint>

#include <algorithm>
#include <array>
#include <fstream>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

/// Dependent Header files

/* 
 * 
#include <vulkan/vulkan.h>

#if defined(_WIN32)
  #include <DirectXMath.h>
  #include <d3d11.h>
  #include <d3d12.h>
  #include <vulkan/vulkan_win32.h>
  #include <Windows.h>

  #pragma comment(lib, "d3d11.lib")
#endif

#include <glad/glad.h>
#include <GLFW/glfw3.h>

 */

/// DianYing Interfaces.

#include <spdlog/fmt/fmt.h>
#include <sol2/sol_forward.hpp>

#if defined(NDEBUG) == false && defined(_WIN32) == true
  #define _CRTDBG_MAP_ALLOC
  #define _CRTDBG_MAP_ALLOC_NEW
  #include <stdlib.h>
  #include <crtdbg.h>
#endif

#include <Dy/Helper/GlobalType.h>
#include <Dy/Helper/System/DisableWarning.h>
#include <Dy/Helper/System/Macroes.h>

#endif /// DIANYING_PRECOMPILED_H