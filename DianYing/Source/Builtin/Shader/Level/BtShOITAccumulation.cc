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
#import <Input_DefaultVao>;
#import <Input_UboCamera>;
#import <Etc_Miscellaneous>;
#import <Input_ModelTransform>;

out VS_OUT 
{ 
  vec2  texCoord; 
  vec2  screenTex;
  float zDiffuse;
  float wValue;
  float zValue;
} vs_out;

void main() 
{
  vec4 mdp = DyTransform(vec4(dyPosition, 1.0));
  vec4 vec    = uCamera.mViewMatrix * mdp;
  gl_Position = uCamera.mProjMatrix * vec;

  vs_out.wValue   = vec.w;
  vs_out.texCoord = dyTexCoord0;
  vs_out.zDiffuse = abs((mat3(uCamera.mViewMatrix) * normalize(DyGetRotationMatrix() * dyNormal)).z);
  vs_out.zValue   = DyToZValue(gl_Position.z, gl_Position.w);
  vs_out.screenTex= DoToScreenSpaceXy(gl_Position.xy / gl_Position.w);
}
)dy");

MDY_SET_IMMUTABLE_STRING(sFrag, R"dy(
#version 430
#import <Output_OITStream>;

in VS_OUT 
{ 
  vec2  texCoord; 
  vec2  screenTex;
  float zDiffuse;
  float wValue;
  float zValue;
} vs_out;

uniform float uAlphaOffset;
uniform float uDepthScale;
layout (binding = 0) uniform sampler2D uTexture0;  // unlit

vec4 ShaderFragment()
{
  vec4 color = texture(uTexture0, vs_out.texCoord);
  color.rgb *= vs_out.zDiffuse;
  color.a   *= uAlphaOffset;
  return color;
}

void main()
{
  if (DyIsTransparentObjectFrontOf(vs_out.screenTex, vs_out.zValue) == false)
  {
    discard;
  }

  vec4 color = ShaderFragment();

  // Assuming that the projection matrix is a perspective projection.
  // gl_FragCoord.w returns the inverse of the oPos.w.
  float viewDepth = abs(1.0f / vs_out.wValue);

  // Tuned to work well with accumulation buffer is FP16.
  float linearDepth = viewDepth * uDepthScale;
  // See `Weighted blended Order Independent Transparency` (9)
  float weight = color.a * clamp(0.03 / (1e-5 + pow(linearDepth, 4.0)), 1e-2, 3e3);

  outColor  = vec4(color.rgb, 1.0f) * weight;
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