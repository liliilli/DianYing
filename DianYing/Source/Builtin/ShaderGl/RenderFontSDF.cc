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
#include <Dy/Builtin/ShaderGl/RenderFontSDF.h>
#include <Dy/Core/Component/Internal/ShaderType.h>
#include <Dy/Management/DataInformationManager.h>
#include <Dy/Management/HeapResourceManager.h>

//!
//! Forward declaration
//!

namespace
{

MDY_SET_IMMUTABLE_STRING(sVertexShaderCode, R"dy(
#version 430 core

layout (location = 0) in vec2 dyPosition;
layout (location = 1) in vec2 dyTexCoord0;

uniform mat4 uUiProjMatrix;

out VS_OUT { vec2 texCoord; } vs_out;

void main() {
	gl_Position     = vec4((uUiProjMatrix * vec4(dyPosition, 0, 1)).xy, 0, 1);
	vs_out.texCoord = dyTexCoord0.xy;
}
)dy");

MDY_SET_IMMUTABLE_STRING(sFragmentShaderCode, R"dy(
#version 430 core

in VS_OUT { vec2 texCoord; } fs_in;

layout (binding = 0) uniform sampler2D uCharTexture;
uniform vec4  bgColor;
uniform vec4  fgColor;

layout (location = 0) out vec4 gOutput;

const float u_buffer  = 0.5f;
const float u_gamma   = 0.025f;

void main() {
  float dist    = texture(uCharTexture, fs_in.texCoord).r;
  float alpha   = smoothstep(u_buffer - u_gamma, u_buffer + u_gamma, dist);
  gOutput       = vec4(fgColor.rgb, alpha * fgColor.a);
}
)dy");

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy::builtin
{

FDyBuiltinShaderGLRenderFontSDF::FDyBuiltinShaderGLRenderFontSDF()
{
  PDyShaderConstructionDescriptor shaderDesc;
  shaderDesc.mShaderName = FDyBuiltinShaderGLRenderFontSDF::sName;
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
  MDY_CALL_ASSERT_SUCCESS(rescManager.CreateShaderResource(shaderDesc.mShaderName));
}

} /// ::dy::builtin namesapce