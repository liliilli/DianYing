#ifndef GUARD_DY_MANAGEMENT_TYPE_SOUND_EDySoundStatus_H
#define GUARD_DY_MANAGEMENT_TYPE_SOUND_EDySoundStatus_H
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

/// @enum EDySoundStatus
/// @brief Sound instance status enum type
enum class EDySoundStatus
{
  NotValid, // Sound is not valid because resource is not bound to Dy system yet.
  Play,     // Sound is playing.
  Paused,   // Sound is pausing.
  Stop,     // Sound is end so stopped.
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_TYPE_SOUND_EDySoundStatus_H
