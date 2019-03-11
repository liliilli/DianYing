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
#import <Input_UboCamera>;
#import <Input_UboDirLight>;
#import <Input_UStrPointLight>;

in VS_OUT { vec2 texCoord; } fs_in;

layout (location = 0) out vec4 outColor;

layout (binding = 0) uniform sampler2D uTexture0;       // Unlit
layout (binding = 1) uniform sampler2D uTexture1;       // Normal
layout (binding = 2) uniform sampler2D uTexture2;       // Specular
layout (binding = 3) uniform sampler2D uTexture3;       // Model position, Use it CSM shadowing.
layout (binding = 4) uniform sampler2DArrayShadow uTexture4; // Shadow
layout (binding = 5) uniform sampler2D uTexture5;       // ZValue
layout (binding = 6) uniform sampler2D uTexture6;       // SSAO
layout (binding = 7) uniform sampler2D uTexture7;       // Sky
layout (binding = 8) uniform sampler2D uTexture8;       // Emissive

/*
uniform mat4    uLightVPSBMatrix[4];
uniform vec4    uNormalizedFarPlanes;
//uniform float uShadowBias;
//uniform float uShadowStrength;
*/

vec4 GetNormal()      { return (texture(uTexture1, fs_in.texCoord) - 0.5f) * 2.0f; }
vec4 GetSpecular()    { return (texture(uTexture2, fs_in.texCoord) - 0.5f) * 2.0f; }
vec3 GetModelPos()    { return texture(uTexture3, fs_in.texCoord).xyz; }
vec3 GetEmissive()    { return texture(uTexture8, fs_in.texCoord).xyz; }
float GetZValue()     { return texture(uTexture5, fs_in.texCoord).x; }
float GetSSAOOffset() { return texture(uTexture6, fs_in.texCoord).x; }

vec3 CalculateSpecularColor(vec3 iWorldLightDir, vec3 iWorldNormal, 
                            vec3 iLightRgb, vec3 iSpecularRgb, 
                            float iRoughness, float iIntensity)
{
  const mat3 viewDirectionMatrix = mat3(uCamera.mViewMatrix);

  // Calculate specular value.
  const vec3 viewLightDirection = viewDirectionMatrix * normalize(iWorldLightDir);
  const vec3 viewNormal = viewDirectionMatrix * normalize(iWorldNormal);

  const vec3 cameraView = vec3(0, 0, 1);
  const vec3 halfwayVector = normalize(cameraView + viewLightDirection);
  const float d_slvd_n = 
    pow(
      max(
        dot(viewNormal, halfwayVector)
      , 0.0f)
    , 8 * (1 + 24 * clamp(iRoughness, 0.0f, 1.0f))
  );

  return iSpecularRgb * iLightRgb * (d_slvd_n * iIntensity);
}

float GetHalfLambertFactor(const vec3 iNormal, const vec3 iLight, const float iPow)
{
  return pow(
    max(
      dot(iNormal, iLight)
    , 0) / 2 + 0.5f
  , iPow);
}

vec3 DyCalculateDirectionalLight(const vec3 iUnlit, const vec3 iNormal, const vec4 iSpecular, const vec3 iModelPos)
{
  const float ambientFactor = 0.02f;
  vec3  ambientColor  = ambientFactor * iUnlit;

  // Validation test
  if (length(uLightDir.mDirection) < 0.001) 
  { 
    return ambientColor;
  }

  // Function body (in world space)
  const float lambert = GetHalfLambertFactor(iNormal, uLightDir.mDirection, 2.0f);
  const float diffuseFactor = lambert * uLightDir.mIntensity * 0.03f;
  vec3  diffuseColor  = diffuseFactor * uLightDir.mColor.rgb * iUnlit;

  // Calculate specular value.
  const vec3 specularColor = CalculateSpecularColor(
    uLightDir.mDirection, iNormal,
    uLightDir.mColor.rgb, iSpecular.rgb, iSpecular.a,
    uLightDir.mIntensity * 0.01f);

  vec3 resultColor = ambientColor;
  resultColor += DyComputeShadowCoefficient(uTexture4, iModelPos, GetZValue()) * (diffuseColor);
  resultColor += specularColor; 

  return resultColor;
}

vec3 GetOpaqueColor()
{
  vec3 resultColor    = vec3(0);
  vec4 unlitValue	    = texture(uTexture0, fs_in.texCoord);

  // If alpha is zero, discard but check skymap is activated.
  if (unlitValue.a == 0) 
  { 
    vec4 sky = texture(uTexture7, fs_in.texCoord);
    if (sky.a == 0) { discard; } else { return sky.rgb; }
  }

  vec4 normalValue	 = GetNormal(); 
  vec4 specularValue = GetSpecular();
  vec3 modelPosition = GetModelPos();

  return DyCalculateDirectionalLight(unlitValue.xyz, normalValue.xyz, specularValue, modelPosition);
}

void main()
{
  vec3 opaqueColor = GetOpaqueColor();

  outColor.a    = 1.0f;
  outColor.rgb  = opaqueColor * GetSSAOOffset();

  outColor.rgb += GetEmissive();
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