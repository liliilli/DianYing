#ifndef GUARD_DY_CORE_RESOURCE_INFORMATION_FInformationSound_H
#define GUARD_DY_CORE_RESOURCE_INFORMATION_FInformationSound_H
///
/// MIT License
/// Copyright (c) 2018-2019 Jongmin Yun
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.
///

#include <filesystem>

//!
//! Forward declaration
//!

namespace dy
{
struct PDySoundInstanceMetaInfo;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

/// @class FInformationSound
/// @brief Resource Information of each plain sound.
class FInformationSound final
{
public:
  MDY_ONLY_MOVEABLE_PROPERTIES_DEFAULT(FInformationSound);
  FInformationSound(_MIN_ const PDySoundInstanceMetaInfo& iInput);
  ~FInformationSound() = default;

  /// @brief Get model specifier name.
  const std::string& GetSpecifierName() const noexcept;

  /// @brief Get valid path
  const std::filesystem::path& GetPath() const noexcept;

private:
  const std::string           mSpecifier = MDY_INITIALIZE_EMPTYSTR;
  const std::filesystem::path mPath; 
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_INFORMATION_FInformationSound_H