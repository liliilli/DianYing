#ifndef GUARD_DY_MANAGEMENT_TYPE_SOUND_ADySoundInstance_H
#define GUARD_DY_MANAGEMENT_TYPE_SOUND_ADySoundInstance_H
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

#include <Dy/Management/Type/Sound/EDySoundStatus.h>

namespace dy
{

class ADySoundInstance 
{
public:
  ADySoundInstance() = default;
  virtual ~ADySoundInstance();

  /// @brief Get status value of sound instance.
  MDY_NODISCARD const EDySoundStatus& GetStatus() const noexcept; 
  /// @brief private-function. Set status in internal logic.
  void MDY_PRIVATE(SetStatus)(_MIN_ EDySoundStatus iStatus);

protected:
  EDySoundStatus mSoundStatus = EDySoundStatus::NotValid;
};

inline ADySoundInstance::~ADySoundInstance() = default;

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_TYPE_SOUND_ADySoundInstance_H