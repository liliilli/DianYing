#ifndef GUARD_DY_CORE_THREAD_SDYIOCONNECTIONHELPER_H
#define GUARD_DY_CORE_THREAD_SDYIOCONNECTIONHELPER_H
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

#include <string>
#include <Dy/Core/Thread/EDyIOTask.h>
#include <Dy/Core/Reflection/RDyBuiltinResources.h>

//!
//! Forward declaration
//!

namespace dy
{
enum class EDyScope;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

///
/// @class SDyIOConnectionHelper
/// @brief IO Connection helper class for communicating to IOThread.
///
class SDyIOConnectionHelper final
{
public:
  ///
  /// @brief Create and populate resource which have specifier that can be found in meta information container. \n
  /// If resourceType is `Resource` but failed to find `Information` of populated container,
  /// fist populate `Information` and `Resource` later. \n
  /// If given specifier of resource type could not found in Meta container, causes UB or assertion error in debug mode.
  ///
  static void PopulateResource(_MIN_ const std::string& specifier, _MIN_ EDyResourceType resourceType, _MIN_ EDyResourceStyle resourceStyle, _MIN_ EDyScope scope);

  /// @brief Try resource garbage collection manually. This function may causes time consuming, call this carefully.
  static void TryGC();

  /// @brief Try stop IO thread manually. this function is thread-safe to I/O Thread.
  static void TryStop();
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_THREAD_SDYIOCONNECTIONHELPER_H