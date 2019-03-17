#ifndef GUARD_DY_ELEMENT_TYPE_TIMER_FDYTIMERHANDLE_H
#define GUARD_DY_ELEMENT_TYPE_TIMER_FDYTIMERHANDLE_H
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

/// @calss FTimerHandle
/// @brief Actual timer handle instance type for controlling from scripts.
/// This handle can be used both actor and widget script.
class FTimerHandle final
{
public:
  /// @brief Check handler is bound to manager.
  MDY_NODISCARD bool IsBound() const noexcept { return this->mIsBound; }

  /// @brief Set index. Do not use this function in script.
  void MDY_PRIVATE(SetBinding)(_MIN_ TU32 iIndex) noexcept 
  { 
    this->mIndex   = iIndex; 
    this->mIsBound = true;
  }

  /// @brief Unbind handle from TimerManager. Do not use this function in script.
  void MDY_PRIVATE(Unbind)() 
  { 
    this->mIndex = 0; 
    this->mIsBound = false; 
  }

  /// @brief Get index.
  MDY_NODISCARD TU32 MDY_PRIVATE(GetIndex)() const noexcept { return this->mIndex; }

private:
  TU32 mIndex   = 0;
  bool mIsBound = false;
};

} /// ::dy namespace

#endif /// GUARD_DY_ELEMENT_TYPE_TIMER_FDYTIMERHANDLE_H