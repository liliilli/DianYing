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
#include <Dy/Builtin/ShaderGl/Deprecated/RenderFontSDF_Deprecated.h>
#include <Dy/Core/Component/Internal/ShaderType.h>
#include <Dy/Management/IO/IODataManager.h>
#include <Dy/Management/IO/IOResourceManager.h>

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
uniform vec4  uBgColor;
uniform vec4  uFgColor;
uniform vec4  uEdgeColor;
uniform bool  uIsUsingEdge;
uniform bool  uIsUsingBackground;

layout (location = 0) out vec4 gOutput;

const float uIntUpper  = 0.55f;
const float uIntLower  = 0.4f;
const float uEdgeUpper = 0.05f;
const float uEdgeLimit = 0.00001f;

void main() {
  float dist    = texture(uCharTexture, fs_in.texCoord).r;
  float alpha   = 1.0f;
  vec4  color   = vec4(0);
  vec4  egColor = vec4(0);
  vec4  bkColor = vec4(0);
  if (uIsUsingEdge == true)       { egColor = uEdgeColor; }
  if (uIsUsingBackground == true) { bkColor = uBgColor; }

  if (uIntLower < dist)
  {
    alpha = smoothstep(uIntLower, uIntUpper, dist);
    color = uFgColor * alpha + egColor * (1 - alpha);
  }
  else
  {
    alpha = smoothstep(uEdgeLimit, uEdgeUpper, dist);
    color = egColor * alpha + bkColor * (1 - alpha);
  }

  gOutput = color;
}
)dy");

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy::builtin
{

FDyBuiltinShaderGLRenderFontSDF_Deprecated::FDyBuiltinShaderGLRenderFontSDF_Deprecated()
{
  PDyShaderConstructionDescriptor shaderDesc;
  shaderDesc.mShaderName = FDyBuiltinShaderGLRenderFontSDF_Deprecated::sName;
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

  MDY_CALL_ASSERT_SUCCESS(infoManager.CreateShaderInformation_Deprecated(shaderDesc));
  MDY_CALL_ASSERT_SUCCESS(rescManager.CreateShaderResource(shaderDesc.mShaderName));
}

} /// ::dy::builtin namesapce