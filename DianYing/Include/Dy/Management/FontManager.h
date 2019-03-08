#ifndef GUARD_DY_MANAGEMENT_FONTMANAGER_H
#define GUARD_DY_MANAGEMENT_FONTMANAGER_H
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

#include <Dy/Management/Interface/ISingletonCrtp.h>
#include <Dy/Management/Type/FontResourceContainer.h>
#include <Dy/Helper/Pointer.h>

namespace dy
{

class MDyFont final : public IDySingleton<MDyFont>
{
  MDY_SINGLETON_PROPERTIES(MDyFont);
  MDY_SINGLETON_DERIVED(MDyFont);
public:
  ///
  /// @brief  Create font resource container from meta container.
  /// @param  fontSpecifierName
  /// @return If succeeded return DY_SUCCESS or DY_FAILURE.
  ///
  EDySuccess CreateFontResourceContainer(_MIN_ const std::string& fontSpecifierName);

  ///
  /// @brief  Check there is valid font container with name is fontSpecifierName.
  /// @param  specifierName Font container specifier name.
  /// @return If found just return true or false.
  ///
  MDY_NODISCARD bool IsFontResourceContainerExist(_MIN_ const std::string& specifierName);

  ///
  /// @brief  Remove and release resource of valid font container.
  /// @param  fontSpecifierName
  /// @return If succeeded to remove valid font container, return DY_SUCCESS or DY_FAILURE.
  ///
  MDY_NODISCARD EDySuccess RemoveFontContainer(_MIN_ const std::string& fontSpecifierName);

  ///
  /// @brief  Get font resource container.
  /// @param  specifierName Font container speicfier name.
  /// @return Return default font container reference.
  ///
  MDY_NODISCARD IDyFontContainer* GetFontResourceContainer(_MIN_ const std::string& specifierName);

private:
  using TFontContainerMap = std::unordered_map<std::string, std::unique_ptr<IDyFontContainer>>;

  ///
  TFontContainerMap   mFontResourceContainerMap = {};
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_FONTMANAGER_H