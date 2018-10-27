#ifndef GUARD_DY_MANAGEMENT_FONTMANAGER_H
#define GUARD_DY_MANAGEMENT_FONTMANAGER_H
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

#include <Dy/Management/Interface/ISingletonCrtp.h>
#include <Dy/Management/Type/FontContainer.h>
#include <Dy/Helper/Pointer.h>

namespace dy
{

class MDyFont final : public ISingleton<MDyFont>
{
  MDY_SINGLETON_PROPERTIES(MDyFont);
  MDY_SINGLETON_DERIVED(MDyFont);
public:
  ///
  /// @brief  Create and save font resource container.
  /// @param  fontSpecifierName
  /// @return If succeeded return DY_SUCCESS or DY_FAILURE.
  ///
  MDY_NODISCARD EDySuccess CreateFontContainer(_MIN_ const std::string& fontSpecifierName);

  ///
  /// @brief  Check there is valid font container with name is fontSpecifierName.
  /// @param  fontSpecifierName
  /// @return If fount just return true or false.
  ///
  FORCEINLINE MDY_NODISCARD bool IsFontContainerExist(_MIN_ const std::string& fontSpecifierName)
  {
    return this->mValidFontContainerMap.find(fontSpecifierName) != this->mValidFontContainerMap.end();
  }

  ///
  /// @brief  Remove and release resource of valid font container.
  /// @param  fontSpecifierName
  /// @return If succeeded to remove valid font container, return DY_SUCCESS or DY_FAILURE.
  ///
  MDY_NODISCARD EDySuccess RemoveFontContainer(_MIN_ const std::string& fontSpecifierName);

  ///
  /// @brief  Get Default font container.
  /// @return Return default font container.
  ///
  MDY_NODISCARD NotNull<FDyFontContainer*> GetDefaultFontContainer() const noexcept;

private:
  using TFontContainerMap = std::unordered_map<std::string, FDyFontContainer>;

  ///
  TFontContainerMap mValidFontContainerMap  = {};
  /// Default font container pointer. This will be held pointer when creating manager.
  FDyFontContainer* mDefaultFontContainer   = MDY_INITIALIZE_NULL;
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_FONTMANAGER_H