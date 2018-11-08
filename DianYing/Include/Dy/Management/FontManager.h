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
#include <Dy/Management/Type/FontContainer_Deprecated.h>
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
  MDY_DEPRECATED(0.0.1, CreateFontContainer_Deprecated, CreateFontResourceContainer)
  MDY_NODISCARD EDySuccess CreateFontContainer_Deprecated(_MIN_ const std::string& fontSpecifierName);

  ///
  /// @brief  Create font resource container from meta container.
  /// @param  fontSpecifierName
  /// @return If succeeded return DY_SUCCESS or DY_FAILURE.
  ///
  MDY_NODISCARD EDySuccess CreateFontResourceContainer(_MIN_ const std::string& fontSpecifierName);

  ///
  /// @brief  Check there is valid font container with name is fontSpecifierName.
  /// @param  fontSpecifierName
  /// @return If fount just return true or false.
  ///
  FORCEINLINE MDY_NODISCARD bool IsFontContainerExist(_MIN_ const std::string& fontSpecifierName)
  {
    return this->mValidFontContainerMap_Deprecated.find(fontSpecifierName) != this->mValidFontContainerMap_Deprecated.end();
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
  MDY_NODISCARD NotNull<FDyFontContainer_Deprecated*> GetDefaultFontContainer() const noexcept;

private:
  using TFontContainerMap = std::unordered_map<std::string, FDyFontContainer_Deprecated>;



  ///
  TFontContainerMap mValidFontContainerMap_Deprecated  = {};
  /// Default font container pointer. This will be held pointer when creating manager.
  FDyFontContainer_Deprecated* mDefaultFontContainer_Deprecated   = MDY_INITIALIZE_NULL;
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_FONTMANAGER_H