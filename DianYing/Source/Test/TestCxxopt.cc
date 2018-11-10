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

/// Header file
#include <Dy/Test/TestCxxopt.h>
#if defined(MDY_FLAG_TEST_ENABLED)

#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

#include <cxxopts.hpp>

namespace dy::test
{

void TestCxxoptLibrary()
{
  cxxopts::Options options("MyProgram", "One line description of MyProgram");

  options.add_options()
      ("g,graphics", "Enable graphics API with", cxxopts::value<std::string>())
      ("c,enable_logging_console", "Enable logging console", cxxopts::value<bool>())
      ("f,enable_logging_file", "Enable logging file to", cxxopts::value<std::string>())
  ;

#if defined(MDY_PLATFORM_FLAG_WINDOWS) && defined(_WIN32)
  const TI32 size = __argc;
  auto result = options.parse(__argc, __argv);
#elif defined(MDY_PLATFORM_FLAG_LINUX) && defined(__linux__)
  static_assert(false, "Linux does not support now.");
#elif defined(MDY_PLATFORM_FLAG_MACOS)
  static_assert(false, "Macos does not support now.");
#endif

  const auto g = result["graphics"].as<std::string>();
  const auto c = result["enable_logging_console"].as<bool>();
  const auto f = result["enable_logging_file"].as<std::string>();
}

} /// ::dy namespace
#endif /// MDY_FLAG_TEST_ENABLED