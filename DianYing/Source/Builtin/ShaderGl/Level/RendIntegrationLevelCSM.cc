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
#include <Dy/Builtin/ShaderGl/Level/RendIntegrationLevelCSM.h>

//!
//! Forward declaration
//!

namespace
{

MDY_SET_IMMUTABLE_STRING(sVert, R"dy(
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

MDY_SET_IMMUTABLE_STRING(sFrag, R"dy(
#version 430

in VS_OUT 
{ 
  vec2 texCoord; 
} fs_in;

layout (location = 0) out vec4 outColor;

layout (binding = 0) uniform sampler2D uTexture0;       // Unlit
layout (binding = 1) uniform sampler2D uTexture1;       // Normal
layout (binding = 2) uniform sampler2D uTexture2;       // View vector
layout (binding = 3) uniform sampler2D uTexture3;       // Modelposition, Use it CSM shadowing.
layout (binding = 4) uniform sampler2DArrayShadow uTexture4; // Shadow
layout (binding = 5) uniform sampler2D uTexture5;       // ZValue
layout (binding = 6) uniform sampler2D uTexture6;       // SSAO
layout (binding = 7) uniform sampler2D uTexture7;       // Sky

uniform mat4  uLightVPSBMatrix[4];
uniform vec4  uNormalizedFarPlanes;
//uniform float uShadowBias;
//uniform float uShadowStrength;

float sShadowBias = 0.001f;

// binding = 1 is DirectionalLightBlock uniform block.
layout(std140, binding = 1) uniform DirectionalLightBlock
{
  vec3  mDirection; // World space
  vec4  mDiffuse;   // Not use alpha value
  vec4  mSpecular;  // Not use alpha value
  vec4  mAmbient;   // Not use alpha value
  float mIntensity; // Intensity
} uLightDir[1];

vec4 layerColor = vec4(1.0, 0.5f, 1.0f, 1.0f); // DEBUG

vec4 GetNormal()    { return (texture(uTexture1, fs_in.texCoord) - 0.5f) * 2.0f; }
vec4 GetSpecular()  { return (texture(uTexture2, fs_in.texCoord) - 0.5f) * 2.0f; }
vec3 GetModelPos()  { return texture(uTexture3, fs_in.texCoord).xyz; }
float GetZValue()   { return texture(uTexture5, fs_in.texCoord).x; }
float GetSSAOOffset() { return texture(uTexture6, fs_in.texCoord).x; }

vec3 ComputeShadowCoords(int iSlice, vec3 iWorldPosition)
{
  // Orthographic projection doesn't need division by w.
  return (uLightVPSBMatrix[iSlice] * vec4(iWorldPosition, 1.0f)).xyz;
}

float ComputeShadowCoefficient(vec3 iWorldPosition, float iZValue)
{
  int slice = 3;
       if (iZValue < uNormalizedFarPlanes.x) { slice = 0; layerColor = vec4(1.0, 0.5, 0.5, 1.0); }
  else if (iZValue < uNormalizedFarPlanes.y) { slice = 1; layerColor = vec4(0.5, 1.0, 0.5, 1.0); }
  else if (iZValue < uNormalizedFarPlanes.z) { slice = 2; layerColor = vec4(0.5, 0.5, 1.0, 1.0); }

  vec4 shadowCoords;
  // Swizzling specific for shadow sampler.
  shadowCoords.xyw = ComputeShadowCoords(slice, iWorldPosition);
  shadowCoords.w  -= sShadowBias;
  shadowCoords.z   = float(slice);
  
  return texture(uTexture4, shadowCoords); 
}

vec3 GetOpaqueColor()
{
  vec3 resultColor    = vec3(0);
  vec4 unlitValue	    = texture(uTexture0, fs_in.texCoord);
  if (unlitValue.a == 0) 
  { // If alpha is zero, discard.
    vec4 sky = texture(uTexture7, fs_in.texCoord);
    if (sky.a == 0) { discard; } else { return sky.rgb; }
  }

  vec4 normalValue	  = GetNormal(); 
  vec4 specularValue  = GetSpecular();
  vec3 modelPosition  = GetModelPos();

  for (int i = 0; i < uLightDir.length; ++i)
  { 
    // Validation test
    if (length(uLightDir[i].mDirection) < 0.001) { continue; }

    // Function body
    float d_n_dl    = dot(normalValue.xyz, uLightDir[i].mDirection);
    vec3  s_l_vd    = normalize(uLightDir[i].mDirection + specularValue.xyz);
    float d_slvd_n  = pow(max(dot(s_l_vd, normalValue.xyz), 0.0f), 32);

    float ambientFactor  = 0.02f;
    vec3  ambientColor   = ambientFactor * uLightDir[i].mAmbient.rgb * unlitValue.rgb;

    float diffuseFactor  = max(d_n_dl, 0.1f) * uLightDir[i].mIntensity * 0.03f;
    vec3  diffuseColor   = diffuseFactor * uLightDir[i].mDiffuse.rgb * unlitValue.rgb;

    float specularFactor = d_slvd_n * uLightDir[i].mIntensity * 0.03f;
    vec3  specularColor  = specularFactor * uLightDir[i].mSpecular.rgb * unlitValue.rgb;

    resultColor  = ambientColor;
    resultColor += clamp(ComputeShadowCoefficient(modelPosition, GetZValue()), 0.1f, 1.0f) * (diffuseColor);
  }
  return resultColor;
}

void main()
{
  vec3 opaqueColor = GetOpaqueColor();

  outColor.a    = 1.0f;
  outColor.rgb  = opaqueColor * GetSSAOOffset();
}
)dy");

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy::builtin
{

FDyBtGlslRenderIntegrationLevelCSM::FDyBtGlslRenderIntegrationLevelCSM()
{
  this->mSpecifierName  = sName;
  this->mVertexBuffer   = sVert;
  this->mPixelBuffer    = sFrag;
}

} /// ::dy::builtin namespace