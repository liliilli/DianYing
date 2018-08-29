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

#include <unordered_map>

#include <glm/glm.hpp>

//!
//! Forward declaration
//!

namespace dy
{
struct DVector2;
struct DVector3;
struct DVector4;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy {

///
/// @brief
///
enum class EDyShaderFragmentType
{
  Vertex,
  TesselationControl,
  TesselationEvaluation,
  Geometry,
  Pixel
};

///
/// @brief Shader internal variable type for uniform and textures.
///
enum class EDyUniformVariableType {
  Matrix4 = 0,
  Matrix3,
  Matrix2,
  Vector4,
  Vector3,
  Vector2,
  IVec4,
  IVec3,
  IVec2,
  Integer,
  IntegerPointer,
  Float,
  FloatPointer,
  Texture2D
};

///
/// @brief
///
struct PDyShaderFragmentInformation final
{
  EDyShaderFragmentType mShaderType;
  std::string           mShaderPath = "";
};

///
/// @brief
///
struct PDyShaderConstructionDescriptor final
{
  std::vector<PDyShaderFragmentInformation> mShaderFragments;
};

///
/// @class CDyShaderComponent
/// @brief New shader wrapper class
///
class CDyShaderComponent final
{
  using TUniformId = int32_t;
  using TUniformStruct = std::tuple<std::string, EDyUniformVariableType, TUniformId>;
public:
  CDyShaderComponent() = default;
  CDyShaderComponent(const CDyShaderComponent&) = delete;
  CDyShaderComponent& operator=(const CDyShaderComponent&) = delete;
  CDyShaderComponent(CDyShaderComponent&&) = default;
  CDyShaderComponent& operator=(CDyShaderComponent&&) = default;
  ~CDyShaderComponent();

  [[nodiscard]]
  EDySuccess pInitializeShaderProgram(const PDyShaderConstructionDescriptor& shaderConstructionDescriptor);

  ///
  /// @brief
  ///
  void UseShader();

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

  ///
  /// @brief
  ///
  void BindShader();

  ///
  /// @brief
  ///
  void UnbindShader();

  ///
  /// @todo TEMPORAL FUNCTION
  ///
  void TempRender();

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
  void SetUniformMatrix4Float(int32_t uniform_id, const glm::mat4& matrix);

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

protected:
#ifdef false
  void pInitializeUniformVariables(const resource::SShader::TVariableList& uniforms);
#endif
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
  std::vector<TUniformStruct> mUniformVariableContainer;

  uint32_t mTemporalVertexArray = 0;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_COMPONENT_SHADER_H