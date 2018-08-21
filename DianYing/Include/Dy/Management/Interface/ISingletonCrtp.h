#ifndef GUARD_DY_MANAGEMENT_INTERFACE_ISINGLETONCRTP_H
#define GUARD_DY_MANAGEMENT_INTERFACE_ISINGLETONCRTP_H
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
/// @log
/// 2018-08-21 Create file.
///

#include <Dy/Helper/macroes.h>

namespace dy
{

template <typename TType>
class ISingleton
{
public:
  ///
  /// @brief Return reference of instance of MDyTime manager.
  /// This function is thread safe.
  ///
  inline static TType& GetInstance() noexcept
  {
    static TType instance;
    return instance;
  }

  ///
  /// @brief
  ///
  [[nodiscard]]
  inline EDySuccess static Initialize() noexcept
  {
    return GetInstance().pfInitialize();
  }

  ///
  /// @brief
  ///
  [[nodiscard]]
  inline EDySuccess static Release() noexcept
  {
    return GetInstance().pfRelease();
  }

protected:
  ISingleton() = default;
  virtual ~ISingleton() = default;

  MDY_SINGLETON_PROPERTIES(ISingleton);
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_INTERFACE_ISINGLETONCRTP_H