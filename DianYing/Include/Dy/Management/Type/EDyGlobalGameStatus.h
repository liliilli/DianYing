#ifndef GUARD_DY_MANAGEMENT_TYPE_EDYGLOBALGAMESTATUS_H
#define GUARD_DY_MANAGEMENT_TYPE_EDYGLOBALGAMESTATUS_H
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

namespace dy
{

///
/// @enum EDyGlobalGameStatus
/// @brief Global game status for synchronization from MDySynchronization.
///
enum class EDyGlobalGameStatus : char
{
  // Original status.
  None,
  // Initial status, load loading textures synchronously.
  Booted,
  // After loaded loading resource, load global resources (builtin) and (customized)
  FirstLoading,
  // After loaded global resources, load scene object resource and scene scope resources (asynchronously).
  Loading,
  // After loaded scene resources, go to runtime.
  GameRuntime,
  // If game need to be shutdowned, go to game runtime.
  Shutdown,
  // Game end. just exit application.
  Ended
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_TYPE_EDYGLOBALGAMESTATUS_H