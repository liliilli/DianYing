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
#include <Dy/Management/Interface/IFontContainer.h>
#include <Dy/Helper/System/Pointer.h>

namespace dy
{

class MFont final : public ISingleton<MFont>
{
public:
  MDY_SINGLETON_PROPERTIES(MFont);
  MDY_SINGLETON_DERIVED(MFont);
  /// @brief  Create font resource container from meta container.
  /// @param  identifier
  /// @return If succeeded return DY_SUCCESS or DY_FAILURE.
  EDySuccess CreateFontResourceContainer(const std::string& identifier);

  /// @brief  Check there is valid font container with name is fontSpecifierName.
  /// @param  identifier Font container specifier name.
  /// @return If found just return true or false.
  MDY_NODISCARD bool IsFontResourceContainerExist(const std::string& identifier);

  /// @brief  Remove and release resource of valid font container.
  /// @param  identifier
  /// @return If succeeded to remove valid font container, return DY_SUCCESS or DY_FAILURE.
  EDySuccess RemoveFontContainer(const std::string& identifier);

  /// @brief  Get font resource container.
  /// @param  identifier Font container speicfier name.
  /// @return Return default font container reference.
  IFontContainer* GetFontResourceContainer(const std::string& identifier);

private:
  /// @brief Container type for supporting polymorphic font containers.
  using TFontContainerMap = std::unordered_map<std::string, std::unique_ptr<IFontContainer>>;

  /// @brief Container
  TFontContainerMap mFontResourceContainerMap = {};
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_FONTMANAGER_H