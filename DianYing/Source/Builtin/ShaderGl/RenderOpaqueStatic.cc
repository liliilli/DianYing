#include <precompiled.h>
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

/// Header file
#include <Dy/Builtin/ShaderGl/RenderOpaqueStatic.h>
#include <Dy/Core/Component/Internal/ShaderType.h>
#include <Dy/Management/IODataManager.h>
#include <Dy/Management/IOResourceManager.h>

//!
//! Forward declaration
//!

namespace
{

MDY_SET_IMMUTABLE_STRING(sVertexShaderCode, R"dy(
#version 430

layout (location = 0) in vec3 dyPosition;
layout (location = 1) in vec3 dyNormal;
layout (location = 2) in vec2 dyTexCoord0;

layout(std140, binding = 0) uniform CameraBlock
{
  uniform mat4 mProjMatrix;
  uniform mat4 mViewMatrix;
} uCamera;

uniform mat4 modelMatrix;

out gl_PerVertex { vec4 gl_Position; };
out VS_OUT
{
	vec3 fragColor;
	vec3 normal;
	vec2 texCoord;
	vec4 modelPosition;
} vs_out;

void main()
{
  gl_Position			      = uCamera.mProjMatrix * uCamera.mViewMatrix * modelMatrix * vec4(dyPosition, 1.0);
  vs_out.fragColor	    = dyNormal * 0.5f + 0.5f;
	vs_out.normal		      = mat3(modelMatrix) * dyNormal;
	vs_out.texCoord		    = dyTexCoord0;
	vs_out.modelPosition  = modelMatrix * vec4(dyPosition, 1.0);
}

)dy");

MDY_SET_IMMUTABLE_STRING(sFragmentShaderCode, R"dy(
#version 430

in VS_OUT {
	vec3 fragColor;
	vec3 normal;
	vec2 texCoord;
	vec4 modelPosition;
} fs_in;

layout (location = 0) out vec4 gUnlit;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gSpecular;
layout (location = 3) out vec4 gPosition;

uniform sampler2D uTexture0;

void main() {
	gUnlit	  = vec4(texture(uTexture0, fs_in.texCoord).rgb, 1.0f);
	gNormal	  = vec4(normalize(fs_in.normal) * 0.5f + 0.5f, 1.0f);
	gSpecular = vec4(1, 0, 1, 1);
	gPosition = fs_in.modelPosition;
}
)dy");

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy::builtin
{

FDyBuiltinShaderGLRenderOpaqueStatic::FDyBuiltinShaderGLRenderOpaqueStatic()
{
  PDyShaderConstructionDescriptor shaderDesc;
  shaderDesc.mShaderName = FDyBuiltinShaderGLRenderOpaqueStatic::sName;
  {
    PDyShaderFragmentInformation vs;
    vs.mShaderType = EDyShaderFragmentType::Vertex;
    vs.mShaderRawCode = sVertexShaderCode;
    vs.mIsEnabledRawLoadShaderCode = true;
    shaderDesc.mShaderFragments.emplace_back(vs);
  }
  {
    PDyShaderFragmentInformation fs;
    fs.mShaderType = EDyShaderFragmentType::Pixel;
    fs.mShaderRawCode = sFragmentShaderCode;
    fs.mIsEnabledRawLoadShaderCode = true;
    shaderDesc.mShaderFragments.emplace_back(fs);
  }

  auto& infoManager = MDyIOData::GetInstance();
  auto& rescManager = MDyIOResource::GetInstance();

  MDY_CALL_ASSERT_SUCCESS(infoManager.CreateShaderInformation(shaderDesc));
  MDY_CALL_ASSERT_SUCCESS(rescManager.CreateShaderResource(MSVSTR(FDyBuiltinShaderGLRenderOpaqueStatic::sName)));
}

} /// ::dy::builtin namespace