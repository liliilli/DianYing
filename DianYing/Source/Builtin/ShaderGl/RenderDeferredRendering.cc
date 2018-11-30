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

//!
//! Forward declaration
//!

namespace
{

MDY_SET_IMMUTABLE_STRING(sVertexShaderCode, R"dy(
#version 430

layout (location = 0) in vec3 dyPosition;
layout (location = 2) in vec2 dyTexCoord0;

out gl_PerVertex { vec4 gl_Position; };
out VS_OUT { vec2 texCoord; } vs_out;

void main() {
	vs_out.texCoord	= dyTexCoord0;
  gl_Position			= vec4(dyPosition, 1.0);
}
)dy");

MDY_SET_IMMUTABLE_STRING(sFragmentShaderCode, R"dy(
#version 430

in VS_OUT { vec2 texCoord; } fs_in;
layout (location = 0) out vec4 outColor;

uniform sampler2D uUnlit;
uniform sampler2D uNormal;
uniform sampler2D uSpecular;     // View vector
uniform sampler2D uModelPosition;
uniform sampler2D uShadow;

uniform mat4 uShadowPv;

float sBias = 0.02f;

// binding = 0 is view, project matrix
// binding = 1 is DirectionalLightBlock uniform block.
layout(std140, binding = 1) uniform DirectionalLightBlock
{
  vec3  mDirection; // World space
  vec4  mDiffuse;   // Not use alpha value
  vec4  mSpecular;  // Not use alpha value
  vec4  mAmbient;   // Not use alpha value
  float mIntensity; // Intensity
} uLightDir[5];

void main()
{
  vec3 resultColor    = vec3(0);
  vec4 unlitValue	    = texture(uUnlit, fs_in.texCoord);
  if (unlitValue.a == 0) { discard; }

  vec4 normalValue	  = (texture(uNormal, fs_in.texCoord) - 0.5f) * 2.0f;
  vec4 specularValue  = (texture(uSpecular, fs_in.texCoord) - 0.5f) * 2.0f;
  vec4 modelPos       = uShadowPv * texture(uModelPosition, fs_in.texCoord);
  modelPos            = modelPos / modelPos.w;
  modelPos            = modelPos * 0.5f + 0.5f;
  float closestDepth  = texture(uShadow, modelPos.xy).r;

  for (int i = 0; i < uLightDir.length; ++i)
  { // Integrity test
    if (length(uLightDir[i].mDirection) < 0.001) { continue; }

    // Function body
    float d_n_dl    = dot(normalValue.xyz, uLightDir[i].mDirection);
    vec3  s_l_vd    = normalize(uLightDir[i].mDirection + specularValue.xyz);
    float d_slvd_n  = pow(max(dot(s_l_vd, normalValue.xyz), 0.0f), 32);

    float ambientFactor   = 0.05f;
    vec3  ambientColor    = ambientFactor * uLightDir[i].mAmbient.rgb;

    float diffuseFactor   = max(d_n_dl, 0.1f) * uLightDir[i].mIntensity;
    vec3  diffuseColor    = diffuseFactor * uLightDir[i].mDiffuse.rgb;

    float specularFactor  = d_slvd_n * uLightDir[i].mIntensity;
    vec3  specularColor   = specularFactor * uLightDir[i].mSpecular.rgb;

    float shadingOffset   = 1.f;
    if (closestDepth < modelPos.z) { shadingOffset = 0.5f; }

    resultColor += (ambientColor + diffuseColor + specularColor) * unlitValue.rgb * shadingOffset;
  }

  outColor = vec4(resultColor.rgb, 1.0f);
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
  this->mSpecifierName  = sName;
  this->mVertexBuffer   = sVertexShaderCode;
  this->mPixelBuffer    = sFragmentShaderCode;
}

} /// ::dy::builtin namespace