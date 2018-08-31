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

#include <Dy/Core/Component/Internal/ShaderType.h>

//!
//! Forward declaration
//!

namespace dy
{
class CDyShaderInformation;
struct DVector2;
struct DVector3;
struct DVector4;
class DDyMatrix4x4;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy {

///
/// @class CDyShaderResource
/// @brief New shader wrapper class
///
class CDyShaderResource final
{
  using TUniformId = int32_t;
  using TUniformStruct = std::tuple<std::string, EDyUniformVariableType, TUniformId>;
public:
  CDyShaderResource() = default;
  CDyShaderResource(const CDyShaderResource&) = delete;
  CDyShaderResource& operator=(const CDyShaderResource&) = delete;
  CDyShaderResource(CDyShaderResource&&) = default;
  CDyShaderResource& operator=(CDyShaderResource&&) = default;
  ~CDyShaderResource();

  ///
  /// @brief Turn on shader program.
  ///
  void UseShader() noexcept;

  ///
  /// @brief
  ///
  void BindShader() noexcept;

  ///
  /// @brief
  ///
  void UnbindShader() noexcept;

  ///
  /// @brief Turn off shader program.
  ///
  void UnuseShader() noexcept;

  ///
  /// @brief
  ///
  void UpdateUniformVariables();

  ///
  /// @brief Get shader program id.
  /// @return Program id.
  ///
  uint32_t GetShaderProgramId() const noexcept
  {
    return mShaderProgramId;
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
  void SetUniformVector2Float(int32_t uniform_id, const DVector2& vector);

  ///
  /// @brief The method sets $ R^3 $ vector to arbitary uniform variable.
  ///
  void SetUniformVector3Float(int32_t uniform_id, const DVector3& vector);

  ///
  /// @brief The method sets $ R^4 $ vector to arbitary uniform variable.
  ///
  void SetUniformVector4Float(int32_t uniform_id, const DVector4& vector);

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
  [[nodiscard]]
  EDySuccess pfInitializeResource(const CDyShaderInformation& shaderInformation);

  ///
  /// @brief
  ///
  EDySuccess pInitializeShaderFragments(const PDyShaderConstructionDescriptor& shaderConstructionDescriptor,
                                        std::vector<std::pair<EDyShaderFragmentType, uint32_t>>& shaderFragmentIdList);

  ///
  /// @brief
  ///
  EDySuccess pInitializeShaderProgram(const std::vector<std::pair<EDyShaderFragmentType, uint32_t>>& shaderFragmentIdList);

  uint32_t mShaderProgramId = 0;
  uint32_t mTemporalVertexArray = 0;

  std::vector<TUniformStruct> mUniformVariableContainer;

  ///
  /// @brief
  ///
  void __pfSetPrevLevel(CDyShaderInformation* ptr) const noexcept { mPrevLevelPtr = ptr; }
  mutable CDyShaderInformation* mPrevLevelPtr = nullptr;

  friend class CDyShaderInformation;
  friend class MDyResource;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_COMPONENT_SHADER_H