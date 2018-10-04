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
#include <Dy/Builtin/ShaderGl/RenderDeferredRendering.h>
#include <Dy/Core/Component/Internal/ShaderType.h>
#include <Dy/Management/DataInformationManager.h>
#include <Dy/Management/HeapResourceManager.h>

//!
//! Forward declaration
//!

namespace
{

MDY_SET_IMMUTABLE_STRING(sVertexShaderCode, R"dy(
#version 430

layout (location = 0) in vec3 dyPosition;
layout (location = 1) in vec2 dyTexCoord0;

out gl_PerVertex { vec4 gl_Position; };
out VS_OUT { vec2 texCoord; } vs_out;

void main() {
	vs_out.texCoord		= dyTexCoord0;
    gl_Position			= vec4(dyPosition, 1.0);
}
)dy");

MDY_SET_IMMUTABLE_STRING(sFragmentShaderCode, R"dy(
#version 430

in VS_OUT { vec2 texCoord; } fs_in;
layout (location = 0) out vec4 outColor;

uniform sampler2D uUnlit;
uniform sampler2D uNormal;
uniform sampler2D uSpecular;
uniform sampler2D uViewPosition;

vec3 dirLight		= normalize(vec3(-1, 1, 0));
vec3 ambientColor	= vec3(1);

void main() {

	vec4 normalValue	= (texture(uNormal, fs_in.texCoord) - 0.5f) * 2.0f;
	vec4 unlitValue		= texture(uUnlit, fs_in.texCoord);

	float ambientFactor = 0.1f;
	float diffuseFactor = max(dot(normalValue.xyz, dirLight), 0.1);

    outColor = vec4(
		vec3(1) * diffuseFactor +
		ambientColor * ambientFactor,
		1.0f);
}
)dy");

} /// ::unname namespace

//!
//! Implementation
//!

namespace dy::builtin
{

FDyBuiltinShaderGLRenderDeferredRendering::FDyBuiltinShaderGLRenderDeferredRendering()
{
  PDyShaderConstructionDescriptor shaderDesc;
  shaderDesc.mShaderName = FDyBuiltinShaderGLRenderDeferredRendering::sName;
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

  auto& infoManager = MDyDataInformation::GetInstance();
  auto& rescManager = MDyHeapResource::GetInstance();

  MDY_CALL_ASSERT_SUCCESS(infoManager.CreateShaderInformation(shaderDesc));
  MDY_CALL_ASSERT_SUCCESS(rescManager.CreateShaderResource(FDyBuiltinShaderGLRenderDeferredRendering::sName.data()));
}

} /// ::dy::builtin namespace