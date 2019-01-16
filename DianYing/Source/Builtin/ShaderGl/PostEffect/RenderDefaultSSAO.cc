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
#include <Dy/Builtin/ShaderGl/PostEffect/RenderDefaultSSAO.h>

//!
//! Forward declaration
//!

namespace
{

MDY_SET_IMMUTABLE_STRING(sVertexShaderCode, R"dy(
#version 430 core

layout (location = 0) in vec3 dyPosition;
layout (location = 2) in vec2 dyTexCoord0;

out VS_OUT { vec2 texCoord; } vs_out;

void main() {
	vs_out.texCoord	= dyTexCoord0;
  gl_Position	= vec4(dyPosition, 1.0);
}
)dy");

MDY_SET_IMMUTABLE_STRING(sFragmentShaderCode, R"dy(
#version 430 core

layout (location = 0) out vec4 oOcclusion;

in VS_OUT { vec2 texCoord; } vs_out;

layout(std140, binding = 0) uniform CameraBlock
{
  uniform mat4 mProjMatrix;
  uniform mat4 mViewMatrix;
} uCamera;

layout (binding = 0) uniform sampler2D uModelNormal;
layout (binding = 1) uniform sampler2D uModelPosition;
layout (binding = 2) uniform sampler2D uNoise;

uniform vec3 uRaySamples[64];
int   uKernelSize	= 64;
float uRadius	    = 3.0f;
float uBias	      = 0.025f;
vec2  uScreenSize      = vec2(1280, 720);
vec2  sNoiseScale;

vec3 GetViewPosition()  { return (uCamera.mViewMatrix * texture(uModelPosition, vs_out.texCoord)).xyz; }
vec3 GetViewNormal()    { return mat3(uCamera.mViewMatrix) * texture(uModelNormal, vs_out.texCoord).xyz; }
vec3 GetRandomVector()  { return normalize(texture(uNoise, vs_out.texCoord * sNoiseScale)).xyz; }

mat3 GetTangentViewMatrix(vec3 iRandomVec, vec3 iNormal)
{
  vec3 tangent    = normalize(iRandomVec - iNormal * dot(iRandomVec, iNormal));
  vec3 bitangent  = cross(iNormal, tangent);
  // T, B, N this matrix convert tangent space to view space.
  return mat3(tangent, bitangent, iNormal);
}

vec3 GetSampleViewPosition(mat3 iTBN, vec3 iRaySample, vec3 iOriginPos)
{
  return iOriginPos + (iTBN * iRaySample) * uRadius;
}

vec2 GetSampleNDCUV(vec3 iSampleViewPos)
{
  vec4 offset = vec4(iSampleViewPos, 1.0);
  offset      = uCamera.mProjMatrix * offset;
  offset.xyz /= offset.w;
  offset.xyz	= offset.xyz * 0.5 + 0.5;

  return offset.xy;
}

float GetSampleViewDepth(vec2 iSampleUV)
{
  return (uCamera.mViewMatrix * texture(uModelPosition, iSampleUV)).z;
}

void main()
{
	sNoiseScale  = uScreenSize / 4.0f;

  vec3 viewPos    = GetViewPosition();
  vec3 normal     = GetViewNormal();
  vec3 randomVec  = GetRandomVector();
  mat3 TBN        = GetTangentViewMatrix(randomVec, normal);

  float occlusionIntensity = 0.0f;
  for (int i = 0; i < uKernelSize; ++i)
  {
    vec3 sampleViewPos    = GetSampleViewPosition(TBN, uRaySamples[i], viewPos); // get sample position
    float sampleViewDepth	= GetSampleViewDepth(GetSampleNDCUV(sampleViewPos));
    float rangeCheck	    = smoothstep(0.0, 1.0, uRadius / abs(viewPos.z - sampleViewDepth));
    occlusionIntensity += (sampleViewDepth >= sampleViewPos.z + uBias ? 1.0f : 0.0f) * rangeCheck;
  }

  oOcclusion = vec4(1.0 - (occlusionIntensity / uKernelSize));
}
)dy");

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy::builtin
{

FDyBuiltinShaderGLRenderDefaultSSAO::FDyBuiltinShaderGLRenderDefaultSSAO()
{
  this->mSpecifierName  = sName;
  this->mVertexBuffer   = sVertexShaderCode;
  this->mPixelBuffer    = sFragmentShaderCode;
}

} /// ::dy::builtin namespace