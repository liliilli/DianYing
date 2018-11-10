#ifndef GUARD_DY_CORE_ETC_STATICCORETRIGGER_H
#define GUARD_DY_CORE_ETC_STATICCORETRIGGER_H
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

namespace dy
{

///
/// @struct SDyCoreTrigger
/// @brief  Static core trigger helper type. To begin and end the game, static function will be triggered.
///
struct SDyCoreTrigger final
{
  ///
  /// @brief Initialize all managers of `Dy` Core. \n
  /// This function must be called only before everything related runtime started.
  ///
  static void InitiailzeAllManagers();

  ///
  /// @brief Release all managers of `Dy` Core.
  /// This function must be called only after everything related runtime is done.
  ///
  static void ReleaseAllManagers();

  SDyCoreTrigger()                                  = delete;
  ~SDyCoreTrigger()                                 = delete;
  SDyCoreTrigger(const SDyCoreTrigger&)             = delete;
  SDyCoreTrigger& operator=(const SDyCoreTrigger&)  = delete;
  SDyCoreTrigger(SDyCoreTrigger&&)                  = delete;
  SDyCoreTrigger& operator=(SDyCoreTrigger&&)       = delete;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_ETC_STATICCORETRIGGER_H