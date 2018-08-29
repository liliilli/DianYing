#ifndef GUARD_DY_MANAGEMENT_HANDLE_MANAGER_H
#define GUARD_DY_MANAGEMENT_HANDLE_MANAGER_H
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

#include <unordered_map>

#include <Dy/Management/Interface/ISingletonCrtp.h>
#include <Dy/Core/Component/Internal/ShaderType.h>
#include <Dy/Core/Component/Internal/TextureType.h>

#include <Dy/Core/Component/Shader.h>
#include <Dy/Core/Component/Texture.h>

//!
//! Forward declaration
//!


//!
//! Implementation
//!

namespace dy
{

class MDyHandle final : public dy::ISingleton<MDyHandle>
{
  MDY_SINGLETON_DERIVED(MDyHandle);
  MDY_SINGLETON_PROPERTIES(MDyHandle);

  template <typename TType>
  using THashList = std::unordered_map<std::string, TType>;
public:
  ///
  /// @brief
  ///
  EDySuccess CreateShaderResource(const std::string& shaderName, const PDyShaderConstructionDescriptor& shaderDescriptor);

  ///
  /// @brief
  ///
  EDySuccess CreateTextureResource(const std::string& textureName, const PDyTextureConstructionDescriptor& textureDescriptor);

  ///
  /// @brief
  ///
  CDyShaderComponent* GetShaderResource(const std::string& shaderName);

  ///
  /// @brief
  ///
  CDyTextureComponent* GetTextureResource(const std::string& shaderName);

private:
  THashList<std::unique_ptr<CDyShaderComponent>>  mOnBoardShaderLists;
  THashList<std::unique_ptr<CDyTextureComponent>> mOnBoardTextureLists;
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_HANDLE_MANAGER_H