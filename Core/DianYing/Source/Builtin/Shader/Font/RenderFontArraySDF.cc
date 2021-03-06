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

#include <Dy/Builtin/ShaderGl/Font/RenderFontArraySDF.h>

//!
//! Forward declaration
//!

namespace
{

MDY_SET_IMMUTABLE_STRING(sVertexShaderCode, R"dy(
#version 430 core

layout (location = 0) in vec2   dyPosition;
layout (location = 1) in vec2   dyTexCoord0;
layout (location = 2) in vec2   dyChanMap;

uniform mat4 uUiProjMatrix;

out VS_OUT { 
  vec2  texCoord; 
  flat int channel;
  flat int mapIndex;
} vs_out;

void main() {
	gl_Position     = vec4((uUiProjMatrix * vec4(dyPosition, 0, 1)).xy, 0, 1);

	vs_out.texCoord = dyTexCoord0.xy;
  vs_out.channel  = int(dyChanMap.x);
  vs_out.mapIndex = int(dyChanMap.y);
}
)dy");

MDY_SET_IMMUTABLE_STRING(sFragmentShaderCode, R"dy(
#version 430 core

in VS_OUT { 
  vec2 texCoord; 
  flat int channel;
  flat int mapIndex;
} fs_in;

layout (binding = 0) uniform sampler2DArray uCharTexture;

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
  int uChannel = fs_in.channel;
  float dist      = 0.0f;
  vec3  texCoord  = vec3(fs_in.texCoord, fs_in.mapIndex);
       if (uChannel == 0) { dist = texture(uCharTexture, texCoord).r; }
  else if (uChannel == 1) { dist = texture(uCharTexture, texCoord).g; }
  else if (uChannel == 2) { dist = texture(uCharTexture, texCoord).b; }
  else if (uChannel == 3) { dist = texture(uCharTexture, texCoord).a; }

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

FDyBuiltinShaderGLRenderFontArraySDF::FDyBuiltinShaderGLRenderFontArraySDF()
{
  this->mSpecifierName  = sName;
  this->mVertexBuffer   = sVertexShaderCode;
  this->mPixelBuffer    = sFragmentShaderCode;
}

} /// ::dy namespace