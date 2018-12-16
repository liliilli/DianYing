#ifndef GUARD_DY_CORE_COMPONENT_SHADER_H
#define GUARD_DY_CORE_COMPONENT_SHADER_H
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

#include <Dy/Core/Resource/Internal/ShaderType.h>
#include "Dy/Core/Resource/Internal/MaterialType.h"

//!
//! Forward declaration
//!

namespace dy
{
class   DDyShaderInformation_Deprecated;
class   CDyMaterialResource_Deprecated;
struct  DDyVector2;
struct  DDyVector3;
struct  DDyVector4;
class   DDyMatrix4x4;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy {

///
/// @class CDyShaderResource_Deprecated_Deprecated
/// @brief Shader resource class which manages shader non-safe heap instance.
///
class CDyShaderResource_Deprecated final
{
  using TUniformId = int32_t;
  using TUniformStruct = std::tuple<std::string, EDyUniformVariableType, TUniformId>;
public:
  CDyShaderResource_Deprecated() = default;
  CDyShaderResource_Deprecated(const CDyShaderResource_Deprecated&) = delete;
  CDyShaderResource_Deprecated& operator=(const CDyShaderResource_Deprecated&) = delete;
  CDyShaderResource_Deprecated(CDyShaderResource_Deprecated&&) = default;
  CDyShaderResource_Deprecated& operator=(CDyShaderResource_Deprecated&&) = default;
  ~CDyShaderResource_Deprecated();

  ///
  /// @brief Turn on shader program.
  ///
  void UseShader() noexcept;

  ///
  /// @brief Turn off shader program.
  ///
  void UnuseShader() noexcept;

  ///
  /// @brief Update uniform variable values into shader.
  ///
  void UpdateUniformVariables();

  ///
  /// @brief Get activated plain attribute variable information list.
  ///
  [[nodiscard]]
  const auto& GetAttributeVariableInformationList() const noexcept
  {
    return this->mAttributeVariableLists;
  }

  ///
  /// @brief Get activated plain uniform variable information list.
  ///
  [[nodiscard]]
  const auto& GetUniformVariableInformationList() const noexcept
  {
    return this->mPlainUniformVariableLists;
  }

  ///
  /// @brief Get shader program id.
  /// @return Program id.
  ///
  [[nodiscard]] uint32_t GetShaderProgramId() const noexcept
  {
    return this->mShaderProgramId;
  }

  //!
  //! uniform functions (ONLY OPENGL!)
  //!

  ///
  /// @brief The method sets singed-integer value to arbitary uniform variable.
  ///
  void SetUniformInteger(int32_t uniform_id, int32_t value) const;

  ///
  /// @brief Set signed-integer value C-array pointer with amount to arbitary uniform variable.
  /// @param[in] value Value to be inputted.
  /// @param[in] size Amount how many values to be inputted.
  ///
  void SetUniformIntegerPointer(int32_t uniform_id, int32_t* value, int32_t size) const;

  ///
  /// @brief The method sets float value to arbitary uniform variable.
  /// The method sets float (single-precision real) value to arbitary uniform variable.
  ///
  void SetUniformFloat(int32_t uniform_id, float value) const;

  ///
  /// @brief Set float value C-array pointer with amount to arbitary uniform variable.
  /// @param[in] value Value to be inputted.
  ///
  void SetUniformFloatPointer(int32_t uniform_id, float* value, int32_t size) const;

  ///
  /// @brief The method sets $ R^2 $ vector to arbitary uniform variable.
  ///
  void SetUniformVector2Float(int32_t uniform_id, const DDyVector2& vector);

  ///
  /// @brief The method sets $ R^3 $ vector to arbitary uniform variable.
  ///
  void SetUniformVector3Float(int32_t uniform_id, const DDyVector3& vector);

  ///
  /// @brief The method sets $ R^4 $ vector to arbitary uniform variable.
  ///
  void SetUniformVector4Float(int32_t uniform_id, const DDyVector4& vector);

  ///
  /// @brief The method sets $ \mathbf{M}_{4x4} $ matrix to
  /// arbitary uniform variable.
  ///
  void SetUniformMatrix4Float(int32_t uniform_id, const DDyMatrix4x4& matrix);

  ///
  /// @brief The method sets singed-integer value to arbitary uniform variable.
  /// @param[in] name The name of uniform variable to be specified.
  /// @param[in] value value to be inputted.
  ///
  void SetUniformInteger(const std::string& name, int32_t value) const;

  ///
  /// @brief Set signed-integer value C-array pointer with amount to arbitary uniform variable.
  /// @param[in] name The name of uniform variable to be specified.
  /// @param[in] value Value to be inputted.
  /// @param[in] amount Amount how many values to be inputted.
  ///
  void SetUniformIntegerPointer(const std::string& name, int32_t* value, int32_t amount) const;

  ///
  /// @brief The method sets float value to arbitary uniform variable.
  /// The method sets float (single-precision real) value to arbitary uniform variable.
  /// @param[in] name The name of uniform variable to be specified.
  /// @param[in] value value to be inputted.
  ///
  void SetUniformFloat(const std::string& name, float value) const;

#ifdef false
  /**
   * @brief The method sets light::DirectionalLight information to shader uniform variable.
   * @param[in] name The name of uniform variable to be specified.
   * @param[in] container DirectionalLight container instance.
   */
  void SetStructDirLight(const std::string& name, const light::DirectionalLight& container);

  /**
   * @brief The method sets light::PointLight information to shader uniform variable.
   * @param[in] name The name of uniform variable to be specified.
   * @param[in] container PointLight container instance.
   */
   void SetStructPointLight(const std::string& name, const light::PointLight& container);

  /**
   * @brief The method sets light::Spotlight information to shader uniform variable.
   * @param[in] name THe name of uniform variable to be specified.
   * @param[in] container Spotlight container instance.
   */
   void SetStructSpotlight(const std::string& name, const light::Spotlight& container);
#endif

private:
#ifdef false
  void pInitializeUniformVariables(const resource::SShader::TVariableList& uniforms);
#endif

  ///
  /// @brief private-friend function, initialize shader resource with information.
  ///
  MDY_NODISCARD EDySuccess pfInitializeResource(const DDyShaderInformation_Deprecated& shaderInformation);

  /// Initialize shader fragments.
  MDY_NODISCARD EDySuccess __pInitializeShaderFragments(
      const PDyShaderConstructionDescriptor& shaderConstructionDescriptor,
      std::vector<std::pair<EDyShaderFragmentType, uint32_t>>& shaderFragmentIdList
  );

  /// Link fragments and make shader program.
  MDY_NODISCARD
  EDySuccess __pInitializeShaderProgram(const std::vector<std::pair<EDyShaderFragmentType, uint32_t>>& shaderFragmentIdList);

  /// Get shader attribute variables information and save to member list.
  MDY_NODISCARD EDySuccess __pStoreAttributePropertiesOfProgram() noexcept;

  /// Get shader uniform variables information and save to member list.
  MDY_NODISCARD EDySuccess __pStoreConstantUniformPropertiesOfProgram() noexcept;

  ///
  MDY_NODISCARD EDySuccess __pStoreUniformBufferObjectPropertiesOfProgram() noexcept;

  std::string mShaderName           = "";
  uint32_t    mShaderProgramId      = 0;

  std::vector<TUniformStruct>                   mUniformVariableContainer;
  /// Shader attribute variable list  <Name, ByteSize, Type and Id>
  std::vector<DDyAttributeVariableInformation>  mAttributeVariableLists;
  /// Shader uniform variable list    <Name, ByteSize, Type and Id>
  std::vector<DDyUniformVariableInformation>    mPlainUniformVariableLists;
  /// Shader uniform buffer object list <Name>
  std::vector<DDyUniformBufferObjectInformation>mUniformBufferObjectList;

  //!
  //! Level pointers binding
  //!

  template <typename TType>
  using TBindPtrMap = std::unordered_map<TType*, TType*>;

  FORCEINLINE void __pfSetShaderInformationLink(NotNull<DDyShaderInformation_Deprecated*> ptr) const noexcept
  {
    this->__mLinkedShaderInformationPtr = ptr;
  }
  FORCEINLINE void __pfResetShaderInformationLink() const noexcept
  {
    this->__mLinkedShaderInformationPtr = nullptr;
  }

              void __pfSetMaterialResourceLink(CDyMaterialResource_Deprecated* ptr) const noexcept;
  FORCEINLINE void __pfResetMaterialResourceLinking(NotNull<CDyMaterialResource_Deprecated*> ptr) const noexcept
  {
    this->__mLinkedMaterialResourcePtrs.erase(ptr);
  }

  MDY_TRANSIENT DDyShaderInformation_Deprecated*             __mLinkedShaderInformationPtr = nullptr;
  MDY_TRANSIENT TBindPtrMap<CDyMaterialResource_Deprecated>  __mLinkedMaterialResourcePtrs;

  friend class DDyShaderInformation_Deprecated;
  friend class CDyMaterialResource_Deprecated;
  friend class MDyIOResource_Deprecated;
  friend class TDyIOWorker;
  friend class TDyIO;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_COMPONENT_SHADER_H