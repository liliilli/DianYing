#include <precompiled.h>
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

/// Header file
#include <Dy/Core/Rendering/Wrapper/XGLWrapper.h>
#include <thread>

#include <Dy/Core/Rendering/Wrapper/PDyGLWindowContextDescriptor.h>
#include <Dy/Core/Rendering/Wrapper/PDyGLTextureDescriptor.h>
#include <Dy/Core/Rendering/Wrapper/PDyGLShaderFragmentDescriptor.h>
#include <Dy/Core/Rendering/Wrapper/PGLBufferDescriptor.h>
#include <Dy/Core/Rendering/Wrapper/PGLAttachmentDescriptor.h>
#include <Dy/Core/Rendering/Wrapper/PDyGLFrameBufferDescriptor.h>
#include <Dy/Core/Rendering/Wrapper/PDyGLVaoBindDescriptor.h>
#include <Dy/Core/Resource/Internal/XShaderTypes.h>
#include <Dy/Meta/Type/Mesh/DGlVaoBindInformation.h>
#include <Dy/Helper/Type/DArea.h>

#include <Dy/Include/GlInclude.h>

#define MDY_GL_NONE 0
#define MDY_GL_NONE_VAO 0
#if defined(NDEBUG) == false
#define MDY_CHECK_OPENGL \
  { const auto _ = glGetError(); MDY_ASSERT_MSG(_ == GL_NO_ERROR, "OpenGL Command failed."); }
#else
#define MDY_CHECK_OPENGL (void)0;
#endif

//!
//! Forward declaration
//! 

namespace
{

MDY_NODISCARD dy::EAttributeVariableType 
DyGlGetAttributeVariableTypeFrom(_MIN_ GLenum type) noexcept
{
  switch (type)
  {
  case GL_BOOL:                             return dy::EAttributeVariableType::Bool;
  case GL_FLOAT:      case GL_DOUBLE:       return dy::EAttributeVariableType::Float;
  case GL_FLOAT_VEC2: case GL_DOUBLE_VEC2:  return dy::EAttributeVariableType::Vector2;
  case GL_FLOAT_VEC3: case GL_DOUBLE_VEC3:  return dy::EAttributeVariableType::Vector3;
  case GL_FLOAT_VEC4: case GL_DOUBLE_VEC4:  return dy::EAttributeVariableType::Vector4;
  case GL_FLOAT_MAT2: case GL_DOUBLE_MAT2:  return dy::EAttributeVariableType::Matrix2;
  case GL_FLOAT_MAT3: case GL_DOUBLE_MAT3:  return dy::EAttributeVariableType::Matrix3;
  case GL_FLOAT_MAT4: case GL_DOUBLE_MAT4:  return dy::EAttributeVariableType::Matrix4;
  case GL_INT:                              return dy::EAttributeVariableType::Integer;
  case GL_INT_VEC2:                         return dy::EAttributeVariableType::IVec2;
  case GL_INT_VEC3:                         return dy::EAttributeVariableType::IVec3;
  case GL_INT_VEC4:                         return dy::EAttributeVariableType::IVec4;
  default: return dy::EAttributeVariableType::NoneError;
  }
}

MDY_NODISCARD dy::EUniformVariableType 
DyGlGetUniformVariableTypeFrom(_MIN_ GLenum type) noexcept
{
  switch (type)
  {
  case GL_BOOL:                             return dy::EUniformVariableType::Bool;
  case GL_FLOAT:      case GL_DOUBLE:       return dy::EUniformVariableType::Float;
  case GL_FLOAT_VEC2: case GL_DOUBLE_VEC2:  return dy::EUniformVariableType::Vector2;
  case GL_FLOAT_VEC3: case GL_DOUBLE_VEC3:  return dy::EUniformVariableType::Vector3;
  case GL_FLOAT_VEC4: case GL_DOUBLE_VEC4:  return dy::EUniformVariableType::Vector4;
  case GL_FLOAT_MAT2: case GL_DOUBLE_MAT2:  return dy::EUniformVariableType::Matrix2;
  case GL_FLOAT_MAT3: case GL_DOUBLE_MAT3:  return dy::EUniformVariableType::Matrix3;
  case GL_FLOAT_MAT4: case GL_DOUBLE_MAT4:  return dy::EUniformVariableType::Matrix4;
  case GL_INT:                              return dy::EUniformVariableType::Integer;
  case GL_UNSIGNED_INT:                     return dy::EUniformVariableType::Unsigned;
  case GL_INT_VEC2:                         return dy::EUniformVariableType::IVec2;
  case GL_INT_VEC3:                         return dy::EUniformVariableType::IVec3;
  case GL_INT_VEC4:                         return dy::EUniformVariableType::IVec4;
  case GL_SAMPLER_1D:                       return dy::EUniformVariableType::Texture1D;
  case GL_SAMPLER_2D:                       return dy::EUniformVariableType::Texture2D;
  case GL_SAMPLER_2D_RECT:                  return dy::EUniformVariableType::Texture2DRectangle;
  case GL_SAMPLER_2D_ARRAY:                 return dy::EUniformVariableType::Texture2DArray;
  case GL_SAMPLER_2D_ARRAY_SHADOW:          return dy::EUniformVariableType::Texture2DShadowArray;
  case GL_SAMPLER_CUBE:                     return dy::EUniformVariableType::Texture2DCubemap;
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(dy::EUniformVariableType::NoneError);
  }
}

/// @brief 
void CbGlFeatBlendStack(const bool& iTopStatus)
{
  if (iTopStatus == true) { glEnable(GL_BLEND); } 
  else                    { glDisable(GL_BLEND); }
}

/// @brief
void CbGlFeatCullfaceStack(const bool& iTopStatus)
{
  if (iTopStatus == true) { glEnable(GL_CULL_FACE); } 
  else                    { glDisable(GL_CULL_FACE); }
}

/// @brief
void CbGlFeatDepthTestStack(const bool& iTopStatus)
{
  if (iTopStatus == true) { glEnable(GL_DEPTH_TEST); } 
  else                    { glDisable(GL_DEPTH_TEST); }
}

/// @brief
void CbGlFeatScissorTestStack(const bool& iTopStatus)
{
  if (iTopStatus == true) { glEnable(GL_SCISSOR_TEST); } 
  else                    { glDisable(GL_SCISSOR_TEST); }
}

/// @brief
void CbGlPolygonModeStack(const dy::DGlGlobalStates::DPolygonMode& iTopStatus)
{
  // Get value from structure.
  const auto& polygonMode = iTopStatus;
  GLenum mode   = GL_NONE;
  GLenum value  = GL_NONE;

  // Set mode
  switch (polygonMode.mMode)
  {
  case dy::DGlGlobalStates::DPolygonMode::EMode::Front: mode = GL_FRONT;  break;
  case dy::DGlGlobalStates::DPolygonMode::EMode::Back:  mode = GL_BACK;   break;
  case dy::DGlGlobalStates::DPolygonMode::EMode::FrontAndBack: mode = GL_FRONT_AND_BACK; break;
  }
  // Set value
  switch (polygonMode.mValue)
  {
  case dy::DGlGlobalStates::DPolygonMode::EValue::Triangle: value = GL_FILL; break;
  case dy::DGlGlobalStates::DPolygonMode::EValue::Line:     value = GL_LINE; break;
  case dy::DGlGlobalStates::DPolygonMode::EValue::Point:    value = GL_POINT; break;
  }

  // Issue into OpenGL system.
  glPolygonMode(mode, value);
}

void CbGlBlendModeStatus(const dy::DGlGlobalStates::DBlendMode& iTopStatus)
{
  const auto& blendMode = iTopStatus;

  for (TU32 i = 0, size = static_cast<TU32>(blendMode.mBlendingSettingList.size());
       i < size; ++i)
  {
    const auto& [equation, src, dst] = blendMode.mBlendingSettingList[i];
    switch (equation)
    {
    case dy::DGlGlobalStates::DBlendMode::EEqut::SrcAddDst: glBlendEquationi(i, GL_FUNC_ADD); break;
    case dy::DGlGlobalStates::DBlendMode::EEqut::SrcSubDst: glBlendEquationi(i, GL_FUNC_SUBTRACT); break;
    case dy::DGlGlobalStates::DBlendMode::EEqut::DstSubSrc: glBlendEquationi(i, GL_FUNC_REVERSE_SUBTRACT); break;
    case dy::DGlGlobalStates::DBlendMode::EEqut::CompareMin: glBlendEquationi(i, GL_MIN); break;
    case dy::DGlGlobalStates::DBlendMode::EEqut::CompareMax: glBlendEquationi(i, GL_MAX); break;
    }

    GLenum srcEnum = GL_NONE, dstEnum = GL_NONE;
    switch (src)
    {
    case dy::DGlGlobalStates::DBlendMode::EFunc::Zero: srcEnum = GL_ZERO; break;
    case dy::DGlGlobalStates::DBlendMode::EFunc::One:  srcEnum = GL_ONE; break;
    case dy::DGlGlobalStates::DBlendMode::EFunc::SrcColor: srcEnum = GL_SRC_COLOR; break;
    case dy::DGlGlobalStates::DBlendMode::EFunc::OneMinusSrcColor: srcEnum = GL_ONE_MINUS_SRC_COLOR; break;
    case dy::DGlGlobalStates::DBlendMode::EFunc::SrcAlpha: srcEnum = GL_SRC_ALPHA; break;
    case dy::DGlGlobalStates::DBlendMode::EFunc::OneMinusSrcAlpha: srcEnum = GL_ONE_MINUS_SRC_ALPHA; break;
    }
    switch (dst)
    {
    case dy::DGlGlobalStates::DBlendMode::EFunc::Zero: dstEnum = GL_ZERO; break;
    case dy::DGlGlobalStates::DBlendMode::EFunc::One:  dstEnum = GL_ONE; break;
    case dy::DGlGlobalStates::DBlendMode::EFunc::SrcColor: dstEnum = GL_SRC_COLOR; break;
    case dy::DGlGlobalStates::DBlendMode::EFunc::OneMinusSrcColor: dstEnum = GL_ONE_MINUS_SRC_COLOR; break;
    case dy::DGlGlobalStates::DBlendMode::EFunc::SrcAlpha: dstEnum = GL_SRC_ALPHA; break;
    case dy::DGlGlobalStates::DBlendMode::EFunc::OneMinusSrcAlpha: dstEnum = GL_ONE_MINUS_SRC_ALPHA; break;
    }

    glBlendFunci(i, srcEnum, dstEnum);
  }
}

void CbGlCullfaceModeStack(const dy::DGlGlobalStates::DCullfaceMode& iTopStatus)
{
  using EValue = dy::DGlGlobalStates::DCullfaceMode::EValue;
  switch (iTopStatus.mValue)
  {
  case EValue::Front:         glCullFace(GL_FRONT);           break;
  case EValue::Back:          glCullFace(GL_BACK);            break;
  case EValue::FrontAndBack:  glCullFace(GL_FRONT_AND_BACK);  break;
  }
}

void CbGlViewportStack(const dy::DGlGlobalStates::DViewport& iTopStatus)
{
  for (auto& [index, area] : iTopStatus.mViewportSettingList)
  {
    if (index <= -1)  { dy::XGLWrapper::SetViewport(area); }
    else              { dy::XGLWrapper::SetViewportIndexed(static_cast<TU32>(index), area); }
  }
}

void CbGlAttachmentBlendingStack(const std::vector<dy::PBlendingEquation>& iTopStatus)
{
  using namespace dy;
  for (size_t i = 0, size = iTopStatus.size(); i < size; ++i)
  {
    const auto& [mode, src, dst, color] = iTopStatus[i];
    const auto glMode     = PBlendingEquation::ToGLenum(mode);
    const auto glSrcFunc  = PBlendingEquation::ToGLenum(src);
    const auto glDstsFunc = PBlendingEquation::ToGLenum(dst);

    const auto glColorRgba = color.ToArray();

    glBlendEquationi(i, glMode);
    glBlendFunci(i, glSrcFunc, glDstsFunc);
    glBlendColor(glColorRgba[0], glColorRgba[1], glColorRgba[2], glColorRgba[3]);
  }
}

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy
{

std::mutex XGLWrapper::mGLCriticalSectionMutex;

FCallStack<DGlGlobalStates> XGLWrapper::mInternalGlobalStatusStack;
FCallStack<bool> XGLWrapper::mInternal_FeatBlendStack;
FCallStack<bool> XGLWrapper::mInternal_FeatCullfaceStack;
FCallStack<bool> XGLWrapper::mInternal_FeatDepthTestStack;
FCallStack<bool> XGLWrapper::mInternal_FeatScissorTestStack;
FCallStack<DGlGlobalStates::DPolygonMode>   XGLWrapper::mInternal_PolygonModeStack;
FCallStack<DGlGlobalStates::DBlendMode>     XGLWrapper::mInternal_BlendModeStack;
FCallStack<DGlGlobalStates::DCullfaceMode>  XGLWrapper::mInternal_CullfaceModeStack;
FCallStack<DGlGlobalStates::DViewport>      XGLWrapper::mInternal_ViewportStack;
FCallStack<std::vector<PBlendingEquation>>    XGLWrapper::sAttachmentBlendings;

GLFWwindow* XGLWrapper::CreateGLWindow(const PDyGLWindowContextDescriptor& descriptor)
{
  MDY_ASSERT_MSG(descriptor.mWindowName.empty() == false, "Window name must not be empty.");
  MDY_ASSERT_MSG(descriptor.mWindowSize.X > 0 && descriptor.mWindowSize.Y > 0, "Window size must be valid.");

  if (descriptor.mIsUsingDefaultDoubleBuffer == true) { glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE); }
  if (descriptor.mIsUsingDefaultMSAA == true)         { glfwWindowHint(GLFW_SAMPLES, 4); }
  if (descriptor.mIsWindowShouldFocus == true)        { glfwWindowHint(GLFW_FOCUSED, GL_TRUE); }

  if (descriptor.mIsWindowResizable == true)          { glfwWindowHint(GLFW_RESIZABLE, GL_TRUE); }
  else                                                { glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); }
  if (descriptor.mIsWindowVisible == true)            { glfwWindowHint(GLFW_VISIBLE, GL_TRUE); }
  else                                                { glfwWindowHint(GLFW_VISIBLE, GL_FALSE); }

  GLFWmonitor* ptrMonitor = nullptr;
  if (descriptor.mIsWindowFullScreen == true) { ptrMonitor = glfwGetPrimaryMonitor(); }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Set callback function for global internal status stack.
  mInternal_FeatBlendStack.SetCallback(CbGlFeatBlendStack);
  mInternal_FeatDepthTestStack.SetCallback(CbGlFeatDepthTestStack);
  mInternal_FeatCullfaceStack.SetCallback(CbGlFeatCullfaceStack);
  mInternal_FeatScissorTestStack.SetCallback(CbGlFeatScissorTestStack);
  mInternal_PolygonModeStack.SetCallback(CbGlPolygonModeStack);
  mInternal_BlendModeStack.SetCallback(CbGlBlendModeStatus);
  mInternal_CullfaceModeStack.SetCallback(CbGlCullfaceModeStack);
  mInternal_ViewportStack.SetCallback(CbGlViewportStack);
  sAttachmentBlendings.SetCallback(CbGlAttachmentBlendingStack);

  return glfwCreateWindow(
      descriptor.mWindowSize.X, descriptor.mWindowSize.Y,
      descriptor.mWindowName.c_str(),
      ptrMonitor, descriptor.mSharingContext);
}

void XGLWrapper::CreateGLContext(GLFWwindow* window)
{
  glfwMakeContextCurrent(window);
}

std::optional<TU32> XGLWrapper::CreateTexture(const PDyGLTextureDescriptor& descriptor)
{
  // Validation check.
  MDY_ASSERT_MSG(descriptor.mImageFormat != GL_NONE, "Texture Image format must be specified.");
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(descriptor.mPtrBuffer), "Texture Image buffer must not be null.");
  MDY_ASSERT_MSG(descriptor.mTextureSize.X > 0 && descriptor.mTextureSize.Y > 0, "Texture size must be positive value.");
  MDY_ASSERT_MSG(descriptor.mType != ETextureStyleType::NoneError, "Texture Image type must be specified.");
  if (descriptor.mIsUsingCustomizedParameter == true)
  {
    MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(descriptor.mPtrParameterList), "Parameter list must not be null.");
    MDY_ASSERT_MSG(DyCheckTextureParameterList(*descriptor.mPtrParameterList) == EDySuccess::DY_SUCCESS, "Texture Parameter validation failed.");
  }

  TU32 mTextureResourceId = MDY_INITIALIZE_DEFUINT;

  // Make texture.
  switch (descriptor.mType)
  {
  case ETextureStyleType::D1:
  {
    glGenTextures(1, &mTextureResourceId);
    glBindTexture(GL_TEXTURE_1D, mTextureResourceId);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, descriptor.mTextureSize.X, 0, 
        descriptor.mImageFormat, descriptor.mImagePixelType, descriptor.mPtrBuffer->data());
  } break;
  case ETextureStyleType::D2:
  { // Border parameter must be 0.
    glGenTextures(1, &mTextureResourceId);
    glBindTexture(GL_TEXTURE_2D, mTextureResourceId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, MDY_VECTOR_XY(descriptor.mTextureSize), 0, 
        descriptor.mImageFormat, descriptor.mImagePixelType, descriptor.mPtrBuffer->data());
  } break;
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(std::nullopt);
  }
  MDY_CHECK_OPENGL;

  // Make mipmap by following option.
  const GLenum glTextureType = GlGetLowTextureType(descriptor.mType);
  if (descriptor.mIsUsingDefaultMipmap == true) { glGenerateMipmap(glTextureType); }
  MDY_CHECK_OPENGL;

  // Set texture parameters.
  if (descriptor.mIsUsingCustomizedParameter == true)
  { // Apply parameter option list to attachment.
    bool isUsingClampToBorder = false;
    for (const auto& parameter : *descriptor.mPtrParameterList)
    { // Check there is ClmapToBorder for border coloring and set parameter
      if (parameter.mParameterValue == EDyGlParameterValue::ClampToBorder) { isUsingClampToBorder = true; }
      glTexParameteri(glTextureType, DyGetTexParameterNameValue(parameter.mParameterOption), DyGetTexParameterValueValue(parameter.mParameterValue));
    }

    if (isUsingClampToBorder == true)
    { // If isThisAttachmentUsingClampToBorder is true, apply border color to texture.
      glTexParameterfv(glTextureType, GL_TEXTURE_BORDER_COLOR, descriptor.mBorderColor.Data());
    }
  }
  glBindTexture(glTextureType, 0);
  glFlush();
  MDY_CHECK_OPENGL;

  return mTextureResourceId;
}

std::optional<TU32> XGLWrapper::CreateTexture(const PDyGLTextureCubemapDescriptor& descriptor)
{
  // Validation check.
  MDY_ASSERT_MSG(descriptor.mImageFormat != GL_NONE, "Texture Image format must be specified.");
  MDY_ASSERT_MSG(descriptor.mType == ETextureStyleType::D2Cubemap, "Texture Image type must be D2Cubemap.");
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(descriptor.mTopBuffer),    "Texture Image buffer must not be null.");
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(descriptor.mBottomBuffer), "Texture Image buffer must not be null.");
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(descriptor.mFrontBuffer),  "Texture Image buffer must not be null.");
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(descriptor.mBackBuffer),   "Texture Image buffer must not be null.");
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(descriptor.mRightBuffer),  "Texture Image buffer must not be null.");
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(descriptor.mLeftBuffer),   "Texture Image buffer must not be null.");

  if (descriptor.mIsUsingCustomizedParameter == true)
  {
    MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(descriptor.mPtrParameterList), "Parameter list must not be null.");
    MDY_ASSERT_MSG(DyCheckTextureParameterList(*descriptor.mPtrParameterList) == EDySuccess::DY_SUCCESS, "Texture Parameter validation failed.");
  }

  TU32 mTextureResourceId = MDY_INITIALIZE_DEFUINT;
  { // Critical section.
    

    // Make cubemap texture.  
    glGenTextures(1, &mTextureResourceId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureResourceId);
    //
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, MDY_VECTOR_XY(descriptor.mTopSize), 0, 
          descriptor.mImageFormat, descriptor.mImagePixelType, descriptor.mTopBuffer->data());
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, MDY_VECTOR_XY(descriptor.mBottomSize), 0, 
          descriptor.mImageFormat, descriptor.mImagePixelType, descriptor.mBottomBuffer->data());
    //
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, MDY_VECTOR_XY(descriptor.mRightSize), 0, 
          descriptor.mImageFormat, descriptor.mImagePixelType, descriptor.mRightBuffer->data());
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, MDY_VECTOR_XY(descriptor.mLeftSize), 0, 
          descriptor.mImageFormat, descriptor.mImagePixelType, descriptor.mLeftBuffer->data());
    //
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, MDY_VECTOR_XY(descriptor.mFrontSize), 0, 
          descriptor.mImageFormat, descriptor.mImagePixelType, descriptor.mFrontBuffer->data());
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, MDY_VECTOR_XY(descriptor.mBackSize), 0, 
          descriptor.mImageFormat, descriptor.mImagePixelType, descriptor.mBackBuffer->data());
    MDY_CHECK_OPENGL;

    // Make mipmap by following option.
    const GLenum glTextureType = GlGetLowTextureType(descriptor.mType);
    if (descriptor.mIsUsingDefaultMipmap == true) { glGenerateMipmap(glTextureType); }
    MDY_CHECK_OPENGL;

    // Set texture parameters.
    if (descriptor.mIsUsingCustomizedParameter == true)
    { // Apply parameter option list to attachment.
      bool isUsingClampToBorder = false;
      for (const auto& parameter : *descriptor.mPtrParameterList)
      { // Check there is ClmapToBorder for border coloring and set parameter
        if (parameter.mParameterValue == EDyGlParameterValue::ClampToBorder) { isUsingClampToBorder = true; }
        glTexParameteri(glTextureType, DyGetTexParameterNameValue(parameter.mParameterOption), DyGetTexParameterValueValue(parameter.mParameterValue));
      }

      if (isUsingClampToBorder == true)
      { // If isThisAttachmentUsingClampToBorder is true, apply border color to texture.
        glTexParameterfv(glTextureType, GL_TEXTURE_BORDER_COLOR, descriptor.mBorderColor.Data());
      }
    }
    glBindTexture(glTextureType, 0);
    glFlush();
    MDY_CHECK_OPENGL;
  }

  return mTextureResourceId;
}

void XGLWrapper::DeleteTexture(const TU32 validTextureId)
{
  glDeleteTextures(1, &validTextureId);
}

std::optional<TU32> XGLWrapper::CreateShaderFragment(const PDyGLShaderFragmentDescriptor& descriptor)
{
  TU32 shaderFragmentId;

  // Get OpenGL Shader type
  switch (descriptor.mType)
  {
  case EDyShaderFragmentType::Vertex:   shaderFragmentId = glCreateShader(GL_VERTEX_SHADER);          break;
  case EDyShaderFragmentType::Hull:     shaderFragmentId = glCreateShader(GL_TESS_CONTROL_SHADER);    break;
  case EDyShaderFragmentType::Domain:   shaderFragmentId = glCreateShader(GL_TESS_EVALUATION_SHADER); break;
  case EDyShaderFragmentType::Geometry: shaderFragmentId = glCreateShader(GL_GEOMETRY_SHADER);        break;
  case EDyShaderFragmentType::Pixel:    shaderFragmentId = glCreateShader(GL_FRAGMENT_SHADER);        break;
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(std::nullopt);
  }

  MDY_ASSERT_MSG(shaderFragmentId > 0,                        "Failed to create shader.");
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(descriptor.mPtrBuffer),  "Shader fragment buffer must not be null.");

  // Compile shader fragment.
  glShaderSource(shaderFragmentId, 1, &descriptor.mPtrBuffer, nullptr);
  glCompileShader(shaderFragmentId);
  return shaderFragmentId;
}

void XGLWrapper::DeleteShaderFragment(const TU32 shaderFragmentId)
{
  glDeleteShader(shaderFragmentId);
}

std::optional<TU32> XGLWrapper::CreateShaderProgram(const TFragmentList& fragmentList)
{
  const TU32 shaderProgramId = glCreateProgram();

  // Create GlShaderProgram, and attach fragments to link them.
  for (auto& [shaderFragmentType, shaderFragmentId] : fragmentList) 
  { 
    glAttachShader(shaderProgramId, shaderFragmentId); 
  }
  glLinkProgram(shaderProgramId);
  // Delete obsolete & redundant fragment item.
  for (auto& [shaderFragmentType, shaderFragmentId] : fragmentList) 
  { 
    glDetachShader(shaderProgramId, shaderFragmentId); 
  }

  return shaderProgramId;
}

void XGLWrapper::DeleteShaderProgram(const TU32 shaderProgramId)
{
  glDeleteProgram(shaderProgramId);
}

void XGLWrapper::UseShaderProgram(TU32 iShaderProgramId)
{
  glUseProgram(iShaderProgramId);
}

void XGLWrapper::DisuseShaderProgram()
{
  glUseProgram(0);
}

std::optional<TU32> XGLWrapper::CreateBuffer(const PGLBufferDescriptor& descriptor)
{
  TU32 id = MDY_INITIALIZE_DEFUINT;
  GLenum usage = GL_NONE;
  switch (descriptor.mBufferUsage)
  {
  case EDyMeshUsage::StaticDraw:  usage = GL_STATIC_DRAW;   break;
  case EDyMeshUsage::DynamicDraw: usage = GL_DYNAMIC_DRAW;  break;
  case EDyMeshUsage::StreamDraw:  usage = GL_STREAM_DRAW;   break;
  }

  switch (descriptor.mBufferType)
  {
  case EDirectBufferType::VertexBuffer:
  { // VBO
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    { // Make buffer space first,
      glBufferData(GL_ARRAY_BUFFER, descriptor.mBufferByteSize, 0, usage);
      if (descriptor.mPtrBuffer != nullptr)
      {
        // fill out with buffers.
        auto* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        memcpy(ptr, descriptor.mPtrBuffer, descriptor.mBufferByteSize);
        glUnmapBuffer(GL_ARRAY_BUFFER);
      }
    }
    MDY_CHECK_OPENGL;
    glBindBuffer(GL_ARRAY_BUFFER, MDY_GL_NONE);
    glFlush();
  } break;
  case EDirectBufferType::ElementBuffer:
  { // EBO
    glGenBuffers(1, &id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, descriptor.mBufferByteSize, descriptor.mPtrBuffer, usage);
    MDY_CHECK_OPENGL;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, MDY_GL_NONE);
    glFlush();
  } break;
  case EDirectBufferType::UniformBuffer:
  case EDirectBufferType::TransformFeedback:
  case EDirectBufferType::ShaderStorage:
    MDY_NOT_IMPLEMENTED_ASSERT();
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(std::nullopt);
  }

  return id;
}

void XGLWrapper::DeleteBuffer(const TU32 directBufferId)
{
  glDeleteBuffers(1, &directBufferId);
}

void XGLWrapper::MapBuffer(
  EDirectBufferType iBufferType, TU32 iBufferId, 
  const void* iPtrBuffer, TU32 iBufferSize)
{
  GLenum bufferType = GL_NONE;
  switch (iBufferType)
  {
  case EDirectBufferType::VertexBuffer:   { bufferType = GL_ARRAY_BUFFER; } break;
  case EDirectBufferType::ElementBuffer:  { bufferType = GL_ELEMENT_ARRAY_BUFFER; } break;
  default: MDY_UNEXPECTED_BRANCH(); break;
  }

  glBindBuffer(bufferType, iBufferId);
  glBufferSubData(bufferType, 0, iBufferSize, iPtrBuffer);
}

void XGLWrapper::MapBufferExt(
  EDirectBufferType iBufferType, TU32 iBufferId, const void* iPtrBuffer, TU32 iBufferSize,
  TU32 iItemByteSize, TU32 iGapByteSize, TU32 iStartPoint)
{
  GLenum  bufferType = GL_NONE;
  auto*   ptrInput   = static_cast<const char*>(iPtrBuffer);
  switch (iBufferType)
  {
  case EDirectBufferType::VertexBuffer:   { bufferType = GL_ARRAY_BUFFER; } break;
  case EDirectBufferType::ElementBuffer:  { bufferType = GL_ELEMENT_ARRAY_BUFFER; } break;
  default: MDY_UNEXPECTED_BRANCH(); break;
  }

  glBindBuffer(bufferType, iBufferId);
  char* ptr = static_cast<char*>(glMapBuffer(bufferType, GL_WRITE_ONLY));
  ptr += iStartPoint;
  while (ptrdiff_t(ptrInput) - ptrdiff_t(iPtrBuffer) < iBufferSize)
  {
    memcpy(ptr, ptrInput, iItemByteSize);
    ptr       += iItemByteSize + iGapByteSize;
    ptrInput  += iItemByteSize;
  }
  glUnmapBuffer(bufferType);
}

TU32 XGLWrapper::CreateVertexArrayObject()
{
  TU32 vaoId = MDY_INITIALIZE_DEFUINT;
  glGenVertexArrays(1, &vaoId);
  return vaoId;
}

void XGLWrapper::BindVertexArrayObject(const PDyGLVaoBindDescriptor& iDescriptor)
{
  if (iDescriptor.mAttributeInfo.mIsUsingDefaultDyAttributeModel == true)
  { // If descriptor using default attribute structure binding model, retrieve information from another dimesion
    // and call `BindVertexArrayobject` Function recursively.
    PDyGLVaoBindDescriptor descriptor;
    descriptor.mVaoId         = iDescriptor.mVaoId;
    descriptor.mBoundVboId    = iDescriptor.mBoundVboId;
    descriptor.mBoundEboId    = iDescriptor.mBoundEboId;
    descriptor.mAttributeInfo = GetDefaultAttributeFormatDescriptor();
    BindVertexArrayObject(descriptor);
    return;
  }

  const auto& attributeInfo = iDescriptor.mAttributeInfo;
  const auto attributeCount = attributeInfo.mAttributeFormatList.size();

  glBindVertexArray(iDescriptor.mVaoId);
  glBindBuffer(GL_ARRAY_BUFFER, iDescriptor.mBoundVboId);
  glBindVertexBuffer(0, iDescriptor.mBoundVboId, attributeInfo.mOffsetByteSize, attributeInfo.mStrideByteSize);
  MDY_CHECK_OPENGL;

  if (iDescriptor.mBoundEboId > 0) 
  { 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iDescriptor.mBoundEboId); 
  }

  for (auto i = 0u; i < attributeCount; ++i)
  { // Set up vbo attribute formats.
    const auto&   attributeFormat = attributeInfo.mAttributeFormatList[i];
    const GLenum  type            = DyGetGLTypeFrom(attributeFormat.mType);
    const GLenum  isMustNormalized= attributeFormat.mIsMustNormalized == true ? GL_TRUE : GL_FALSE;

    glEnableVertexAttribArray(i);
    if (type == GL_INT) 
    { 
      glVertexAttribIFormat(i, attributeFormat.mElementCount, type, attributeFormat.mOffsetByteSize); 
    }
    else 
    { 
      glVertexAttribFormat(i, attributeFormat.mElementCount, type, isMustNormalized, 
        attributeFormat.mOffsetByteSize); 
    }
    glVertexAttribBinding(i, 0);
  }
  glVertexBindingDivisor(0, 0);
  MDY_CHECK_OPENGL;

  // If instancing buffer is exist, so should binding instancing id also..
  if (iDescriptor.mInstancingVboId.has_value() == true)
  {
    glEnableVertexAttribArray(10);
    glEnableVertexAttribArray(11);
    glEnableVertexAttribArray(12);
    glEnableVertexAttribArray(13);

    glVertexAttribFormat(10, 4, GL_FLOAT, GL_FALSE, 0);
    glVertexAttribFormat(11, 4, GL_FLOAT, GL_FALSE, 16);
    glVertexAttribFormat(12, 4, GL_FLOAT, GL_FALSE, 32);
    glVertexAttribFormat(13, 4, GL_FLOAT, GL_FALSE, 48);

    glVertexAttribBinding(10, 1);
    glVertexAttribBinding(11, 1);
    glVertexAttribBinding(12, 1);
    glVertexAttribBinding(13, 1);

    glVertexBindingDivisor(1, 1);
    MDY_CHECK_OPENGL;
  }

  glBindVertexArray(MDY_GL_NONE_VAO);
  glFlush();
}

void XGLWrapper::DeleteVertexArrayObject(const TU32 vertexArrayObjectId)
{
  glDeleteVertexArrays(1, &vertexArrayObjectId);
}

std::optional<TU32> XGLWrapper::CreateAttachment(const PGLAttachmentDescriptor& iDescriptor)
{
  // Validation check.
  MDY_ASSERT_MSG(iDescriptor.mBufferSize.X > 0 && iDescriptor.mBufferSize.Y > 0, 
      "Buffer size must be positive value.");
  MDY_ASSERT_MSG(iDescriptor.mBufferFormat != EGlBufferDataInternalFormat::NoneError, 
      "Attachment buffer format must be specified.");
  if (iDescriptor.mIsUsingCustomizedParameter == true)
  {
    MDY_ASSERT_MSG(DyCheckTextureParameterList(iDescriptor.mParameterList) == EDySuccess::DY_SUCCESS, 
        "Attachment Parameter validation failed.");
  }

  TU32 attachmentId = MDY_INITIALIZE_DEFUINT;
  const auto glTextureType    = GlGetLowTextureType(iDescriptor.mAttachmentType);
  const auto mipmapLv         = iDescriptor.mSpecifiedMipmapLevel;
  const auto glInternalFormat = GlGetLowDataFormatType(iDescriptor.mBufferFormat);

  // Create attachment (texture only now)
  glGenTextures(1, &attachmentId);
  glBindTexture(glTextureType, attachmentId);

  switch (iDescriptor.mAttachmentType)
  {
  case ETextureStyleType::D2: 
  case ETextureStyleType::D2Shadow:
  case ETextureStyleType::D2Rectangle:
  {
    glTexStorage2D(glTextureType, mipmapLv, glInternalFormat, 
        MDY_VECTOR_XY(iDescriptor.mBufferSize));
  } break;
  case ETextureStyleType::D1:
  case ETextureStyleType::D1Array: 
  {
    glTexStorage2D(glTextureType, mipmapLv, glInternalFormat, 
        iDescriptor.mBufferSize.X, iDescriptor.mDepthNumber);
  } break;
  case ETextureStyleType::D2Array:
  case ETextureStyleType::D2ShadowArray: 
  {
    glTexStorage3D(glTextureType, mipmapLv, glInternalFormat, 
        MDY_VECTOR_XY(iDescriptor.mBufferSize), iDescriptor.mDepthNumber);
  } break;
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(std::nullopt);
  }

  // Apply parameter option list to attachment.
  bool isThisAttachmentUsingClampToBorder = false;
  for (const auto& parameter : iDescriptor.mParameterList)
  {
    if (parameter.mParameterValue == EDyGlParameterValue::ClampToBorder) { isThisAttachmentUsingClampToBorder = true; }
    glTexParameteri(glTextureType, 
        DyGetTexParameterNameValue(parameter.mParameterOption), 
        DyGetTexParameterValueValue(parameter.mParameterValue));
  }
  if (isThisAttachmentUsingClampToBorder == true)
  {
    glTexParameterfv(glTextureType, GL_TEXTURE_BORDER_COLOR, iDescriptor.mBorderColor.Data());
  }

  glBindTexture(glTextureType, 0);
  glFlush();

  MDY_CHECK_OPENGL;
  return attachmentId;
}

EDySuccess XGLWrapper::DeleteAttachment(TU32 iAttachmentId, bool iIsRenderBuffer)
{
  // Delete attachment (only texture attachment now)
  if (iIsRenderBuffer == true) 
  { 
    glDeleteRenderbuffers(1, &iAttachmentId); 
  }
  else 
  { 
    glDeleteTextures(1, &iAttachmentId); 
  }
  return EDySuccess::DY_SUCCESS;
}

std::optional<TU32> XGLWrapper::CreateFrameBuffer(const PDyGLFrameBufferDescriptor& iDescriptor)
{
  const auto attachmentBindingSize = TU32(iDescriptor.mAttachmentBindingList.size());
  std::vector<GLenum> attachmentTypeList = {};
  attachmentTypeList.reserve(attachmentBindingSize);

  // Make frame buffer resource and attachment.
  TU32 framebufferId = MDY_INITIALIZE_DEFUINT;
  glGenFramebuffers(1, &framebufferId);
  glBindFramebuffer(GL_FRAMEBUFFER, framebufferId);

  for (TU32 i = 0; i < attachmentBindingSize; ++i)
  {
    const auto [attachmentId, attachmentType, attachmentFormat, isRenderBuffer] 
      = iDescriptor.mAttachmentBindingList[i];
    if (isRenderBuffer == false)
    { 
      // If attachment is texture.
      glBindTexture(GlGetLowTextureType(attachmentType), attachmentId);
      const auto typeValue = DyGetAttachmentTypeValue(attachmentFormat);

      glFramebufferTexture(GL_FRAMEBUFFER, typeValue, attachmentId, 0);
      attachmentTypeList.emplace_back(typeValue);
    }
    else
    { // @TODO RENDER BUFFER IS NOT SUPPORTED YET.
      MDY_NOT_IMPLEMENTED_ASSERT();
    }
  }

  if (iDescriptor.mIsUsingDepthBuffer == true)
  { // Bind Depth Buffer
    const auto [depthId, attachmentType, attachmentFormat, isRenderBuffer] = iDescriptor.mDepthBufferBinding;
    if (isRenderBuffer == false)
    {
      glBindTexture(GlGetLowTextureType(attachmentType), depthId);
      glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthId, 0);
    }
    else
    {
      MDY_NOT_IMPLEMENTED_ASSERT();
#ifdef false
      glGenRenderbuffers(1, &depthId);
      glBindRenderbuffer(GL_RENDERBUFFER, depthId);
      glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, MDY_VECTOR_XY(iDescriptor.mFrameBufferSize));
      glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthId);
#endif
    }
  }

  // Let framebuffer know that attachmentBuffer's id will be drawn at framebuffer.
  // @WARNING TODO BE CAREFUL OF INSERTING DEPTH ATTACHMENTS AS COLOR ATTACHMENT!.
  if (iDescriptor.mIsUsingPixelShader == false) 
  { 
    glDrawBuffer(GL_NONE); glReadBuffer(GL_NONE); 
  }
  else 
  { 
    glDrawBuffers(attachmentBindingSize, attachmentTypeList.data()); 
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glFlush();

  MDY_CHECK_OPENGL;
  return framebufferId;
}

EDySuccess XGLWrapper::DeleteFrameBuffer(const TU32 framebufferId)
{
  glDeleteFramebuffers(1, &framebufferId);
  return EDySuccess::DY_SUCCESS;
}

void XGLWrapper::SetViewport(const DArea2D& iViewportRegion)
{
  glViewport(
    static_cast<TI32>(iViewportRegion.GetStartPoint().X), 
    static_cast<TI32>(iViewportRegion.GetStartPoint().Y), 
    static_cast<TI32>(iViewportRegion.GetWidth()), 
    static_cast<TI32>(iViewportRegion.GetHeight())
  );
}

void XGLWrapper::SetViewportIndexed(TU32 iIndex, const DArea2D& iViewportRegion)
{
  glViewportIndexedf(iIndex, 
    iViewportRegion.GetStartPoint().X, 
    iViewportRegion.GetStartPoint().Y, 
    iViewportRegion.GetWidth(), 
    iViewportRegion.GetHeight());
}

void XGLWrapper::BindFrameBufferObject(TU32 iFboId)
{
  glBindFramebuffer(GL_FRAMEBUFFER, iFboId);
}

void XGLWrapper::UnbindFrameBufferObject()
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void XGLWrapper::ClearColorFrameBuffer(TU32 frameBufferId, const DColorRGBA& color, TU32 drawBufferId)
{
  XGLWrapper::BindFrameBufferObject(frameBufferId);
  glClearBufferfv(GL_COLOR, drawBufferId, color.Data());
  XGLWrapper::UnbindFrameBufferObject();
}

void XGLWrapper::ClearDepthFrameBuffer(TU32 frameBufferId, TF32 depthValue)
{
  XGLWrapper::BindFrameBufferObject(frameBufferId);
  glClearBufferfv(GL_DEPTH, 0, &depthValue);
  XGLWrapper::UnbindFrameBufferObject();
}

void XGLWrapper::BindVertexArrayObject(TU32 iVaoId)
{
  glBindVertexArray(iVaoId);
}

void XGLWrapper::UnbindVertexArrayObject()
{
  glBindVertexArray(0);
}

void XGLWrapper::BindTexture(
    TU32 activeTextureIndex, 
    ETextureStyleType type, TU32 textureId)
{
  #if defined(NDEBUG) == false 
  {
    MDY_ASSERT_MSG(glIsTexture(textureId) == GL_TRUE, "given `textureId` is not texture.");
  }
  #endif

  glActiveTexture(GL_TEXTURE0 + activeTextureIndex);
  switch (type)
  {
  case ETextureStyleType::D1: { glBindTexture(GL_TEXTURE_1D, textureId); } break;
  case ETextureStyleType::D2: { glBindTexture(GL_TEXTURE_2D, textureId); } break;
  default: MDY_UNEXPECTED_BRANCH(); break;
  }
}

void XGLWrapper::UnbindTexture(TU32 textureIndex, ETextureStyleType type)
{
  glActiveTexture(GL_TEXTURE0 + textureIndex);
  switch (type)
  {
  case ETextureStyleType::D1: { glBindTexture(GL_TEXTURE_1D, 0); } break;
  case ETextureStyleType::D2: { glBindTexture(GL_TEXTURE_2D, 0); } break;
  default: MDY_UNEXPECTED_BRANCH(); break;
  }
}

void XGLWrapper::Draw(EDrawType iType, bool iIsElement, TU32 iCount)
{
  GLenum drawType = GL_POINT;
  switch (iType)
  {
  case EDrawType::Point:  drawType = GL_POINT;  break;
  case EDrawType::Line:   drawType = GL_LINE;   break;
  case EDrawType::LineStrip: drawType = GL_LINE_STRIP; break;
  case EDrawType::LineLoop:   drawType = GL_LINE_LOOP; break;
  case EDrawType::Triangle:   drawType = GL_TRIANGLES; break;
  case EDrawType::TriangleStip: drawType = GL_TRIANGLE_STRIP; break;
  case EDrawType::TriangleFan:  drawType = GL_TRIANGLE_FAN;   break;
  }

  if (iIsElement == true)
  {
    glDrawElements(drawType, iCount, GL_UNSIGNED_INT, nullptr);
  }
  else
  {
    glDrawArrays(drawType, 0, iCount);
  }
}

void XGLWrapper::DrawInstanced(EDrawType iType, bool iIsElement, TU32 iCount, TU32 iInstanceCount)
{
  GLenum drawType = GL_POINT;
  switch (iType)
  {
  case EDrawType::Point:  drawType = GL_POINT;  break;
  case EDrawType::Line:   drawType = GL_LINE;   break;
  case EDrawType::LineStrip: drawType = GL_LINE_STRIP; break;
  case EDrawType::LineLoop:   drawType = GL_LINE_LOOP; break;
  case EDrawType::Triangle:   drawType = GL_TRIANGLES; break;
  case EDrawType::TriangleStip: drawType = GL_TRIANGLE_STRIP; break;
  case EDrawType::TriangleFan:  drawType = GL_TRIANGLE_FAN;   break;
  }

  if (iIsElement == true)
  {
    glDrawElementsInstanced(drawType, iCount, GL_UNSIGNED_INT, 0, iInstanceCount);
  }
  else
  {
    glDrawArraysInstanced(drawType, 0, iCount, iInstanceCount);
  }
}

TI32 XGLWrapper::QueryShaderProgramIV(TU32 iShaderProgramId, TGlEnum iQueryEnum)
{
  TI32 result = 0;
  glGetProgramiv(iShaderProgramId, iQueryEnum, &result);
  return result;
}

void XGLWrapper::QueryFloatVector(TGlEnum iGLLowEnumCommand, TF32* iPtrRawFloatVector)
{
  glGetFloatv(iGLLowEnumCommand, iPtrRawFloatVector);
}

std::optional<std::tuple<std::string, TGlSize, TI32, EAttributeVariableType, TU32>> 
XGLWrapper::GetShaderProgramAttributeInfo(TU32 iShaderProgramId, TU32 iAttrIndex)
{
  const TI32 attrBufferLength = QueryShaderProgramIV(iShaderProgramId, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH);
  auto* attributeName = static_cast<char*>(std::calloc(attrBufferLength, sizeof(GLchar)));

  GLsizei attributelength = 0;
  GLint   attributeSize   = 0;
  GLenum  attributeType   = GL_NONE;

  glGetActiveAttrib(iShaderProgramId, iAttrIndex, attrBufferLength, &attributelength, &attributeSize, &attributeType, attributeName);
  const TI32 attrLocation = glGetAttribLocation(iShaderProgramId, attributeName);

  MDY_ASSERT_MSG(attrLocation != -1, "Attribute location did not find.");
  const auto type = DyGlGetAttributeVariableTypeFrom(attributeType);
  MDY_ASSERT_MSG(type != EAttributeVariableType::NoneError, "Not supported attribute type.");

  const auto result = std::make_tuple(std::string(attributeName), attributelength, attributeSize, type, attrLocation);
  free(attributeName); attributeName = nullptr;
  return result;
}

std::optional<std::tuple<std::string, TGlSize, TI32, EUniformVariableType, TU32>>
XGLWrapper::GetShaderProgramUniformInfo(TU32 iShaderProgramId, TU32 iUniformIndex)
{
  const TI32 uniformBufLength = QueryShaderProgramIV(iShaderProgramId, GL_ACTIVE_UNIFORM_MAX_LENGTH);
  auto* uniformName = static_cast<char*>(std::calloc(uniformBufLength, sizeof(GLchar)));

  GLsizei uniLength = 0;
  GLint   uniSize   = 0;
  GLenum  uniType   = GL_NONE;
  glGetActiveUniform(iShaderProgramId, iUniformIndex, uniformBufLength, &uniLength, &uniSize, &uniType, uniformName);
  const auto uniLocId = glGetUniformLocation(iShaderProgramId, uniformName);

  if (uniLocId == -1)
  { // Uniform location will be -1 when uniform varaible is in ubo.
    const GLchar* uboNameLastPtr = std::strchr(uniformName, '.');
    MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(uboNameLastPtr), "Unsupported uniform value type.");
    free(uniformName); uniformName = nullptr;
    return std::nullopt;
  }

  auto type = DyGlGetUniformVariableTypeFrom(uniType);
  MDY_ASSERT_MSG(type != EUniformVariableType::NoneError, "Not supported uniform type.");

  // If array type.. 
  if (std::strchr(uniformName, '[') != nullptr)
  {
    switch (type)
    {
    case EUniformVariableType::Matrix4: { type = EUniformVariableType::Matrix4Array; } break;
    case EUniformVariableType::Vector3: { type = EUniformVariableType::Vector3Array; } break;
    case EUniformVariableType::Float:   { type = EUniformVariableType::FloatArray; } break;
    default: MDY_NOT_IMPLEMENTED_ASSERT(); break;
    }
  }

  // Insert
  const auto result = std::make_tuple(std::string(uniformName), uniLength, uniSize, type, uniLocId);
  free(uniformName); uniformName = nullptr;
  return result;
}

std::optional<std::string> 
XGLWrapper::GetShaderProgramUniformBlockInfo(TU32 iShaderProgramId, TU32 iUniformBlockIndex)
{
  const TI32 uboNameMaxLength = QueryShaderProgramIV(iShaderProgramId, GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH);
  auto* uniformName = static_cast<char*>(std::calloc(uboNameMaxLength, sizeof(GLchar)));

  GLsizei uniformLength = 0;
  glGetActiveUniformBlockName(iShaderProgramId, iUniformBlockIndex, uboNameMaxLength, &uniformLength, uniformName);

  if (uniformLength <= 0) 
  { 
    free(uniformName); uniformName = nullptr;
    return std::nullopt; 
  }

  const std::string result = uniformName;
  free(uniformName); uniformName = nullptr;
  return result;
}

void XGLWrapper::UpdateUniformMatrix4(TU32 iId, const DMat4& iBuffer, bool iTransposed)
{
  GLenum transposed = GL_FALSE;
  if (iTransposed == true) { transposed = GL_TRUE; }

  glUniformMatrix4fv(iId, 1, transposed, &iBuffer[0].X);
}

void XGLWrapper::UpdateUniformMatrix3(TU32 iId, const DMat3& iBuffer, bool iTransposed)
{
  GLenum transposed = GL_FALSE;
  if (iTransposed == true) { transposed = GL_TRUE; }

  glUniformMatrix3fv(iId, 1, transposed, &iBuffer[0].X);
}

void XGLWrapper::UpdateUniformMatrix4Array(TU32 iId, const std::vector<DMat4>& iBuffer, bool iIransposed)
{
  if (iBuffer.empty() == true) { return; }

  const TU32 size = static_cast<TU32>(iBuffer.size());
  GLenum transposed = GL_FALSE;
  if (iIransposed == true) { transposed = GL_TRUE; }

  glUniformMatrix4fv(iId, size, transposed, &iBuffer[0][0].X);
}

void XGLWrapper::UpdateUniformVector3Array(TU32 iId, const std::vector<DVec3>& iBuffer)
{
  if (iBuffer.empty() == true) { return; }

  const TU32 size = static_cast<TU32>(iBuffer.size());
  glUniform3fv(iId, size, iBuffer.front().Data());
}

void XGLWrapper::UpdateUniformVector4(TU32 iId, const DVec4& iBuffer)
{
  glUniform4fv(iId, 1, iBuffer.Data());
}

void XGLWrapper::UpdateUniformVector3(TU32 iId, const DVec3& iBuffer)
{
  glUniform3fv(iId, 1, iBuffer.Data());
}

void XGLWrapper::UpdateUniformVector2(TU32 iId, const DVec2& iBuffer)
{
  glUniform2fv(iId, 1, iBuffer.Data());
}

void XGLWrapper::UpdateUniformInteger(TU32 iId, const TI32& iBuffer)
{
  glUniform1i(iId, iBuffer);
}

void XGLWrapper::UpdateUniformUnsigned(TU32 iId, const TU32& iBuffer)
{
  glUniform1ui(iId, iBuffer);
}

void XGLWrapper::UpdateUniformFloat(TU32 iId, const TF32& iBuffer)
{
  glUniform1f(iId, iBuffer);
}

void XGLWrapper::UpdateUniformFloatArray(TU32 iId, const std::vector<TF32>& iBuffer)
{
  if (iBuffer.empty() == true) { return; }

  glUniform1fv(iId, iBuffer.size(), iBuffer.data());
}

void XGLWrapper::PushInternalGlobalState(const DGlGlobalStates& iNewStatus)
{
  //
  mInternalGlobalStatusStack.Push(iNewStatus, false);

  const auto& topStatusChunk = mInternalGlobalStatusStack.Top();
  // Set
  if (topStatusChunk.mIsEnableBlend.has_value() == true)
  { mInternal_FeatBlendStack.Push(*topStatusChunk.mIsEnableBlend); }
  if (topStatusChunk.mIsEnableCullface.has_value() == true)
  { mInternal_FeatCullfaceStack.Push(*topStatusChunk.mIsEnableCullface); }
  if (topStatusChunk.mIsEnableDepthTest.has_value() == true)
  { mInternal_FeatDepthTestStack.Push(*topStatusChunk.mIsEnableDepthTest); }
  if (topStatusChunk.mIsEnableScissorTest.has_value() == true)
  { mInternal_FeatScissorTestStack.Push(*topStatusChunk.mIsEnableScissorTest); }

  if (topStatusChunk.mBlendMode.has_value() == true)
  { mInternal_BlendModeStack.Push(*topStatusChunk.mBlendMode); }
  if (topStatusChunk.mPolygonMode.has_value() == true)
  { mInternal_PolygonModeStack.Push(*topStatusChunk.mPolygonMode); }
  if (topStatusChunk.mCullfaceMode.has_value() == true)
  { mInternal_CullfaceModeStack.Push(*topStatusChunk.mCullfaceMode); }
  if (topStatusChunk.mViewportSettingList.has_value() == true)
  { mInternal_ViewportStack.Push(*topStatusChunk.mViewportSettingList); }
  if (topStatusChunk.mAttachmentBlendings.has_value() == true)
  { sAttachmentBlendings.Push(*topStatusChunk.mAttachmentBlendings); }
}

void XGLWrapper::PopInternalGlobalState()
{
  if (mInternalGlobalStatusStack.IsEmpty() == true) { return; }

  auto extracted = mInternalGlobalStatusStack.ExtractTop(false);

  if (extracted.mIsEnableBlend.has_value() == true)       { mInternal_FeatBlendStack.Pop(); }
  if (extracted.mIsEnableCullface.has_value() == true)    { mInternal_FeatCullfaceStack.Pop(); }
  if (extracted.mIsEnableDepthTest.has_value() == true)   { mInternal_FeatDepthTestStack.Pop(); }
  if (extracted.mIsEnableScissorTest.has_value() == true) { mInternal_FeatScissorTestStack.Pop(); }

  if (extracted.mBlendMode.has_value() == true)     { mInternal_BlendModeStack.Pop(); }
  if (extracted.mPolygonMode.has_value() == true)   { mInternal_PolygonModeStack.Pop(); }
  if (extracted.mCullfaceMode.has_value() == true)  { mInternal_CullfaceModeStack.Pop(); }
  if (extracted.mViewportSettingList.has_value() == true) { mInternal_ViewportStack.Pop(); }
  if (extracted.mAttachmentBlendings.has_value() == true) { sAttachmentBlendings.Pop(); }
}

XGLWrapper::__OutsideLockguard::MDY_PRIVATE(OutsideLockguard)
(XGLWrapper::MDY_PRIVATE(OutsideLockguard)&& iSource) noexcept
{
  iSource.mIsMoved = true;
}

XGLWrapper::__OutsideLockguard XGLWrapper::__LockMutex()
{
  XGLWrapper::mGLCriticalSectionMutex.lock();
  return __OutsideLockguard{};
}

void XGLWrapper::__UnlockMutex()
{
  XGLWrapper::mGLCriticalSectionMutex.unlock();
}

void XGLWrapper::SetupInitialGlobalStatus()
{
  //! Push initial OpenGL global status.
  //! But we don't have to call callback function because it is alreay set on OpenGL system.
  DGlGlobalStates initialStatus;
  {
    {
      using EMode = DGlGlobalStates::DPolygonMode::EMode;
      using EValue = DGlGlobalStates::DPolygonMode::EValue;
      using DPolygonMode = DGlGlobalStates::DPolygonMode;
      // Set value.
      initialStatus.mIsEnableBlend = glIsEnabled(GL_BLEND);
      initialStatus.mIsEnableCullface = glIsEnabled(GL_CULL_FACE);
      initialStatus.mIsEnableDepthTest = glIsEnabled(GL_DEPTH_TEST);
      initialStatus.mIsEnableScissorTest = glIsEnabled(GL_SCISSOR_TEST);
      initialStatus.mPolygonMode = DPolygonMode{ EMode::FrontAndBack, EValue::Triangle };
    }
    // Get blend mode.
    // @TODO DELETE THIS 
    {
      using DBlendMode = DGlGlobalStates::DBlendMode;
      using EEqut = DGlGlobalStates::DBlendMode::EEqut;
      using EFunc = DGlGlobalStates::DBlendMode::EFunc;
      DBlendMode mode{};
      mode.mBlendingSettingList.emplace_back(EEqut::SrcAddDst, EFunc::SrcAlpha, EFunc::OneMinusSrcAlpha);
      initialStatus.mBlendMode = mode;
    }
    // Get cullface mode.
    {
      using DCullfaceMode = DGlGlobalStates::DCullfaceMode;
      DCullfaceMode cullface{ DCullfaceMode::EValue::Back };
      initialStatus.mCullfaceMode = cullface;
    }
    // Get default viewport.
    {
      using DViewport = DGlGlobalStates::DViewport;
      DViewport defaultViewport;
      // Get global size. 
      GLint defaultSize[4]; glGetIntegerv(GL_VIEWPORT, defaultSize);
      defaultViewport.mViewportSettingList.emplace_back(
        -1, // Global 
        DArea2D{ 
          math::Cast<TReal>(defaultSize[0]), math::Cast<TReal>(defaultSize[1]), 
          math::Cast<TReal>(defaultSize[2]), math::Cast<TReal>(defaultSize[3]) }
      );
      initialStatus.mViewportSettingList = defaultViewport;
    }
    // Get default blending equation of default framebuffer.
    {
      using EFunc = PBlendingEquation::EFunc;
      using EMode = PBlendingEquation::EMode;
      GLint blendSrc, blendDst, blendMode;
      glGetIntegerv(GL_BLEND_SRC, &blendSrc);
      glGetIntegerv(GL_BLEND_DST, &blendDst);
      glGetIntegerv(GL_BLEND_EQUATION_RGB, &blendMode);

      // Get wrapped type from glenum.
      const auto defaultMode  = PBlendingEquation::ToMode(blendMode);
      const auto defaultSrc   = PBlendingEquation::ToFunc(blendSrc);
      const auto defaultDst   = PBlendingEquation::ToFunc(blendDst);

      // Get default constant blend color from gl api (this sucks XP)
      std::array<TF32, 4> glDefaultBlendColor;
      glGetFloatv(GL_BLEND_COLOR, glDefaultBlendColor.data());
      const DColorRGBA defaultColor = DColorRGBA{glDefaultBlendColor}; 

      // Insert
      decltype(initialStatus.mAttachmentBlendings)::value_type defaultBlendings;
      defaultBlendings.emplace_back(defaultSrc, defaultMode, defaultDst, defaultColor);
      initialStatus.mAttachmentBlendings = defaultBlendings;
    }

    // Insert default status
    PushInternalGlobalState(initialStatus);
  }
}

XGLWrapper::__OutsideLockguard::~MDY_PRIVATE(OutsideLockguard)() noexcept
{
  if (this->mIsMoved == false)
  { // Unlock gl critical section when not moved.
    glFlush();
    XGLWrapper::MDY_PRIVATE(UnlockMutex)();
  }
}

} /// ::dy namespace