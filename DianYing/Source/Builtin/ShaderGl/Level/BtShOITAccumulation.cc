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
#include <Dy/Builtin/ShaderGl/Level/BtShOITAccumulation.h>

//!
//! Forward declaration
//!

namespace
{

MDY_SET_IMMUTABLE_STRING(sVert, R"dy(
#version 430

layout (location = 0) in vec3 dyPosition;
layout (location = 1) in vec3 dyNormal;
layout (location = 2) in vec2 dyTexCoord0;

layout(std140, binding = 0) uniform CameraBlock
{
  uniform mat4 mProjMatrix;
  uniform mat4 mViewMatrix;
} uCamera;

out VS_OUT 
{ 
  vec2  texCoord; 
  float zDiffuse;
} vs_out;

uniform mat4 uModelMatrix;
uniform mat4 uRotationMatrix;

void main() 
{
  gl_Position     = uCamera.mViewMatrix * uModelMatrix * vec4(dyPosition, 1.0);
  vs_out.texCoord = dyTexCoord0;
  vs_out.zDiffuse = abs(normalize(uRotationMatrix * vec4(dyNormal, 0.0)).z);
}
)dy");

MDY_SET_IMMUTABLE_STRING(sFrag, R"dy(
#version 430

in VS_OUT 
{ 
  vec2  texCoord; 
  float zDiffuse;
} vs_out;

layout (location = 0) out vec4 outColor;
layout (location = 1) out vec4 outWeight;

uniform float uAlphaOffset;
uniform float uDepthScale;
layout (binding = 0) uniform sampler2D uUnlit;

vec4 ShaderFragment()
{
  vec4 color = texture(uUnlit, vs_out.texCoord);
  color.rgb *= vs_out.zDiffuse;
  color.a   *= uAlphaOffset;
  return color;
}

void main()
{
  vec4 color = ShaderFragment();

  // Assuming that the projection matrix is a perspective projection.
  // gl_FragCoord.w returns the inverse of the oPos.w.
  float viewDepth = abs(1.0f / gl_FragCoord.w);

  // Tuned to work well with accumulation buffer is FP16.
  float linearDepth = viewDepth * uDepthScale;
  // See `Weighted blended Order Independent Transparency` (9)
  float weight = clamp(0.03 / (1e-5 + pow(linearDepth, 4.0)), 1e-2, 3e3);

  outColor  = vec4(color.rgb * color.a, color.a) * weight;
  outWeight = vec4(color.a);
}
)dy");

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy::builtin
{

FDyBtShOITAccumulation::FDyBtShOITAccumulation()
{
  this->mSpecifierName = sName;
  this->mVertexBuffer  = sVert;
  this->mPixelBuffer   = sFrag;
}

} /// ::dy::builtin namespace