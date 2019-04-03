#ifndef GUARD_DY_CORE_RESOURCE_RESOURCE_FDYSHADERRESOURCE_H
#define GUARD_DY_CORE_RESOURCE_RESOURCE_FDYSHADERRESOURCE_H
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

#include <vector>
#include <Dy/Meta/Type/EDyResourceTypes.h>
#include <Dy/Core/Resource/Information/FInformationShader.h>
#include <Dy/Core/Resource/Type/TInformationBinder.h>
#include <Dy/Core/Resource/Type/Shader/AUniformValueContainer.h>

//!
//! Forward declaration
//!

namespace dy
{
class FInformationShader;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

/// @class FResourceShader
/// @brief Shader resource class that serve heap instance.
class FResourceShader final : public AUniformValueContainer
{
public:
  using TUniformVariableList  = std::vector<DUniformVariableInformation>;
  using TUniformStructListMap = std::unordered_map<
    std::string, 
    std::pair<std::string, std::vector<DUniformStructVarInformation>>
  >;
  using TUniformStructItemMap = std::unordered_map<
    std::string, 
    std::pair<std::string, DUniformStructVarInformation>>;

  /// @warning input parameter must be atomic!
  FResourceShader(_MIN_ const FInformationShader& information);
  ~FResourceShader();

  /// @brief Get specifier name of valid shader resource.
  const std::string& GetSpecifierName() const noexcept
  {
    return this->mSpecifierName;
  }

  /// @brief Get internal shader program id.
  TU32 GetShaderProgramId() const noexcept;

  /// @brief Turn on shader program.
  void UseShader() const noexcept;

  /// @brief Turn off shader program.
  void DisuseShader() const noexcept;

  /// @brief Get uniform variable specifier list.
  const TUniformVariableList& GetUniformVariableList() const noexcept;
  /// @brief Get uniform struct list map.
  const TUniformStructListMap& GetUniformStructListMap() const noexcept;
  /// @breif Get uniform struct item map.
  const TUniformStructItemMap& GetUniformStructItemMap() const noexcept;

  /// @brief Check shader is supporting instancing.
  MDY_NODISCARD bool IsSupportingInstancing() const noexcept;

private:
  /// @brief Create shader fragment list and return.
  MDY_NODISCARD std::optional<TFragmentList> 
  pCreateShaderFragments(_MIN_ const FInformationShader::TShaderFragmentList& vector);
  /// @brief Create shader program on gpu and return id when it's succeeded.
  MDY_NODISCARD std::optional<TU32> 
  pInitializeShaderProgram(_MIN_ const TFragmentList& fragmentList);
  /// @brief Delete shader fragment list from gpu memory.
  void pDeleteShaderFragments(_MIN_ const TFragmentList& fragmentList);

  void pStoreAttributeProperties() noexcept;
  void pStoreUniformProperties() noexcept;
  void pStoreUniformBufferObjectProperties() noexcept;

  std::string mSpecifierName    = MDY_INITIALIZE_EMPTYSTR;
  TU32        mShaderProgramId;

  /// Shader attribute variable list  <Name, ByteSize, Type and Id>
  std::vector<DAttributeVariableInformation>  mAttributeVariableList;
  /// Shader uniform variable list    <Name, ByteSize, Type and Id>
  TUniformVariableList  mUniformVariableList;
  /// @brief Shader uniform sturcture (not block but structurized) variable list.
  /// <PrefixName, <PrefixName, ..<Name, ByteSize, Type and Id>..>>
  TUniformStructListMap mUniformStructVarListMap;
  /// @brief Shader uniform sturcture (not block but structurized) variable item.
  /// <PrefixName, <PrefixName, <Name, ByteSize, Type and Id>>>
  TUniformStructItemMap mUniformStructVarItemMap;
  /// Shader uniform buffer object list <Name>
  std::vector<DUniformBufferObjectInformation>mUniformBufferObjectList;

  TDyInformationBinderShader mBinderShader;
  bool mIsSupportingInstancing = false;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_RESOURCE_FDYSHADERRESOURCE_H