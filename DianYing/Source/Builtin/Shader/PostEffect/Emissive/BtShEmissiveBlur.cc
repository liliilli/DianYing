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
#include <Dy/Builtin/ShaderGl/PostEffect/Emissive/BtShEmissiveBlur.h>

//!
//! Forward declaration
//!

namespace
{

/*
 * 0.001535	0.003912	0.008934	0.018275	0.033482	0.05495	0.08078	0.106373	0.125472	0.132572	0.125472	0.106373	0.08078	0.05495	0.033482	0.018275	0.008934	0.003912	0.001535
3 으로 나누기
0.016815	0.023597	0.031821	0.041234	0.051343	0.061432	0.07063	0.078033	0.082841	0.084509	0.082841	0.078033	0.07063	0.061432	0.051343	0.041234	0.031821	0.023597	0.016815
5.0 으로 나누기 (*0.2)
 */

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
#import <Etc_Miscellaneous>
/*
layout (binding = 0) uniform sampler2D uTexture0; // uEmissive (RGBA16F);
 */

layout (location = 0) out vec4 oBlurredEmissive;

in VS_OUT { vec2 texCoord; } vs_out;

uniform bool uIsHorizon;
// Sum is 5.0, so must multiply summed value to 0.2.
// http://dev.theomader.com/gaussian-kernel-calculator/
float uBlurWeight[10] = float[]
(0.084509, 0.082841, 0.078033, 0.07063, 0.061432, 0.051343, 0.041234, 0.031821, 0.023597, 0.016815); 

void main()
{
  const vec2 texCoord = vs_out.texCoord;
  const vec3 emissiveColor = texture(uTexture0, texCoord).xyz;
  // gets size of single texel
  const vec2 texOffset = 1.0 / textureSize(uTexture0, 0); 

  vec3 result = emissiveColor * uBlurWeight[0];
  if (uIsHorizon == true)
  {
    for (int i = 1; i < 10; ++i)
    {
      result += texture(uTexture0, texCoord + vec2(texOffset.x * i, 0)).rgb * uBlurWeight[i];
      result -= texture(uTexture0, texCoord - vec2(texOffset.x * i, 0)).rgb * uBlurWeight[i];
    }
  }
  else
  {
    for (int i = 1; i < 10; ++i)
    {
      result += texture(uTexture0, texCoord + vec2(0, texOffset.y * i)).rgb * uBlurWeight[i];
      result -= texture(uTexture0, texCoord - vec2(0, texOffset.y * i)).rgb * uBlurWeight[i];
    }
  }

  result *= 0.5f;
  oBlurredEmissive = vec4(result, 1.0f);
}
)dy");

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy::builtin
{

FDyBuiltinShaderGLEmissiveBlur::FDyBuiltinShaderGLEmissiveBlur()
{
  this->mSpecifierName  = sName;
  this->mVertexBuffer   = sVertexShaderCode;
  this->mPixelBuffer    = sFragmentShaderCode;
}

} /// ::dy::builtin namespace