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
#include <Dy/Builtin/ShaderGl/PostEffect/Emissive/BtShEmissiveToneMap.h>

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
#import <Input_DefaultTexture2D>;
/*
layout (binding = 0) uniform sampler2D uTexture0; // uBlurredEmissive;
layout (binding = 1) uniform sampler2D uTexture1; // uEmissive;
 */

layout (location = 0) out vec4 oResult;

in VS_OUT { vec2 texCoord; } vs_out;

void main()
{
  // DO TONE_MAPPING
  const float gamma = 2.2;
  vec3 hdrColor = texture(uTexture0, vs_out.texCoord).rgb;

  const vec4 origColor = texture(uTexture1, vs_out.texCoord);
  hdrColor = hdrColor * (1 - origColor.a) + origColor.rgb * origColor.a; 

  // reinhard tone mapping
  // lim_{hdrColor -> inf} f(hdrColor) ~= 1.0f.
  vec3 mapped = hdrColor / (hdrColor + vec3(1.0)); 

  // gamma correction 
  mapped = pow(mapped, vec3(1.0 / gamma));
  oResult = vec4(mapped, 1.0); // [0, 1] value.
}
)dy");

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy::builtin
{

FDyBuiltinShaderGLEmissiveToneMap::FDyBuiltinShaderGLEmissiveToneMap()
{
  this->mSpecifierName  = sName;
  this->mVertexBuffer   = sVertexShaderCode;
  this->mPixelBuffer    = sFragmentShaderCode;
}

} /// ::dy::builtin namespace