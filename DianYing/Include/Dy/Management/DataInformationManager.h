#ifndef GUARD_DY_HELPER_MANAGEMENT_RESOURCE_MANAGER_H
#define GUARD_DY_HELPER_MANAGEMENT_RESOURCE_MANAGER_H
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

#include <Dy/Core/Component/Information/ShaderInformation.h>
#include <unordered_map>

namespace dy
{

class MDyDataInformation final : public ISingleton<MDyDataInformation>
{
  MDY_SINGLETON_DERIVED(MDyDataInformation);
  MDY_SINGLETON_PROPERTIES(MDyDataInformation);
public:
  ///
  /// @brief
  ///
  EDySuccess CreateShaderInformation(const std::string& shaderName, const PDyShaderConstructionDescriptor& shaderDescriptor);

  ///
  /// @brief
  ///
  EDySuccess DeleteShaderInformation(const std::string& shaderName);

private:
  ///
  /// @brief Get shader information.
  /// @return
  ///
  [[nodiscard]]
  const CDyShaderInformation* pfGetShaderInformation(const std::string& shaderName) const noexcept;

  std::unordered_map<std::string, std::unique_ptr<CDyShaderInformation>> mShaderInformation;

  friend class MDyResource;
};

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_MANAGEMENT_RESOURCE_MANAGER_H