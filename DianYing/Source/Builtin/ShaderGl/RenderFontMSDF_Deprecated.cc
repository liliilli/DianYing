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
#include <Dy/Builtin/ShaderGl/RenderFontMSDF_Deprecated.h>
#include <Dy/Core/Component/Internal/ShaderType.h>
#include <Dy/Builtin/ShaderGl/RenderDefaultFont.h>
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
uniform float pxRange;
uniform vec4  bgColor;
uniform vec4  fgColor;

layout (location = 0) out vec4 gOutput;

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}

void main() {
    vec2 msdfUnit = pxRange/vec2(textureSize(uCharTexture, 0));
    vec3 samp     = texture(uCharTexture, fs_in.texCoord).rgb;
    float sigDist = median(samp.r, samp.g, samp.b) - 0.5;
    sigDist      *= dot(msdfUnit, 0.5/fwidth(fs_in.texCoord));
    float opacity = clamp(sigDist + 0.5, 0.0, 1.0);

    gOutput = mix(bgColor, fgColor, opacity);
}
)dy");

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy::builtin
{

FDyBuiltinShaderGLRenderFontMSDF_Deprecated::FDyBuiltinShaderGLRenderFontMSDF_Deprecated()
{
  PDyShaderConstructionDescriptor shaderDesc;
  shaderDesc.mShaderName = FDyBuiltinShaderGLRenderFontMSDF_Deprecated::sName;
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

} /// ::dy::builtin namespace