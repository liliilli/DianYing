#ifndef GUARD_DY_EDITOR_INTERFACE_GUI_WINDOW_SINGLETON_H
#define GUARD_DY_EDITOR_INTERFACE_GUI_WINDOW_SINGLETON_H
#if defined(MDY_FLAG_IN_EDITOR)
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

#include <atomic>


#include <Dy/Helper/System/Macroes.h>
#include <Dy/Editor/Interface/IDyGuiComponentBase.h>

namespace dy::editor
{

template <typename TType, typename TConstructionDescriptorType>
class IDyGuiWinSingleton : public IDyGuiComponentBase
{
public:
  IDyGuiWinSingleton()                      = default;
  virtual ~IDyGuiWinSingleton()             = default;

  ///
  /// @brief Initialize singleton.
  ///
  [[nodiscard]] FORCEINLINE EDySuccess Initialize(const TConstructionDescriptorType& desc) noexcept
  {
    const auto flag = static_cast<TType&>(*this).pfInitialize(desc);
    return flag;
  }

  ///
  /// @brief Shutdown singleton.
  ///
  [[nodiscard]] FORCEINLINE EDySuccess Release() noexcept
  {
    const auto flag = static_cast<TType&>(*this).pfRelease();
    return flag;
  }

protected:
  MDY_SINGLETON_PROPERTIES(IDyGuiWinSingleton);
};

} /// ::dy namespace

#endif /// MDY_FLAG_IN_EDITOR
#endif /// GUARD_DY_EDITOR_INTERFACE_GUI_WINDOW_SINGLETON_H