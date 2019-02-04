#ifndef GUARD_DY_HELPER_INTERNAL_NAMEGENERATOR_H
#define GUARD_DY_HELPER_INTERNAL_NAMEGENERATOR_H
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

#include <unordered_map>

namespace dy
{

/// @class FDyNameGenerator
/// @brief This type is container type for generating automated name from given name. \n
/// This type can be used using inheretence or composition.
class FDyNameGenerator
{
public:
  virtual ~FDyNameGenerator() = default;

  /// @brief Try get generated name, when iName is already exist on container, \n
  /// return with suffix `_ + number`. Otherwise, just return parameter name.
  MDY_NODISCARD std::string TryGetGeneratedName(_MIN_ const std::string& iName) noexcept;

private:
  std::unordered_map<std::string, TU32> mContainer;
};

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_INTERNAL_NAMEGENERATOR_H