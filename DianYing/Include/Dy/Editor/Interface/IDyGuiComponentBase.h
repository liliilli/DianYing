#ifndef GUARD_DY_EDITOR_INTERFACE_IDY_GUI_COMPONENT_BASE_H
#define GUARD_DY_EDITOR_INTERFACE_IDY_GUI_COMPONENT_BASE_H
#if defined(MDY_FLAG_IN_EDITOR)
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

#include <Dy/Helper/GlobalType.h>
#include <Dy/Helper/System/HashCompileCrc32.h>
#include <Dy/Helper/System/Macroes.h>

namespace dy::editor
{

///
/// @class IDyGuiComponentBase
/// @brief The base class type of all gui editor components.
///
class IDyGuiComponentBase
{
public:
  virtual ~IDyGuiComponentBase() = default;

  ///
  /// @brief Update gui sequence.
  ///
  virtual void Update(float dt) noexcept {};

  ///
  /// @brief Draw window or gui components.
  ///
  virtual void DrawWindow(float dt) noexcept = 0;

  ///
  /// @brief Return true/false flag whether or not your finding class is this.
  /// @param[in] hashValue Hashed type value of type which you want to find.
  /// @return True/False flag, if you found proper class return true else false.
  ///
  inline virtual bool IsTypeMatched(const TU32 hashValue) const noexcept {
    return __mHashVal == hashValue;
  }

private:
  MDY_SET_CRC32_HASH_WITH_TYPE(IDyGuiComponentBase);
};

} /// ::dy::editor namespace

#endif /// MDY_FLAG_IN_EDITOR
#endif /// GUARD_DY_EDITOR_INTERFACE_IDY_GUI_COMPONENT_BASE_H