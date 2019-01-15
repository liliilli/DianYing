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
#include <Dy/Builtin/ShaderGl/Level/BtShOITIntegration.h>

//!
//! Forward declaration
//!

namespace
{

MDY_SET_IMMUTABLE_STRING(sVert, R"dy(
#version 430

layout (location = 0) in vec2 dyPosition;
layout (location = 1) in vec2 dyTexCoordRect; // TexCoord must can be used for sampler2DRect.
layout (location = 2) in vec2 dyTexCoordScr;  // TexCoord must can be used for sampler2D.

out VS_OUT 
{ 
  vec2 mTexCoordRect;
  vec2 mTexCoordScr; 
} vs_out;

void main() 
{
  gl_Position = vec4(dyPosition, 0.0, 1.0);
  vs_out.mTexCoordRect  = dyTexCoordRect;
  vs_out.mTexCoordScr   = dyTexCoordScr;
}
)dy");

MDY_SET_IMMUTABLE_STRING(sFrag, R"dy(
#version 430

layout (location = 0) out vec4 outColor;

in VS_OUT 
{ 
  vec2 mTexCoordRect;
  vec2 mTexCoordScr; 
} vs_out;

layout (binding = 0) uniform sampler2DRect uTexture0; // Color accumulation
layout (binding = 1) uniform sampler2DRect uTexture1; // Weight accumulation
layout (binding = 2) uniform sampler2D uTexture2;     // Level opaque screen buffer.

void main()
{
  vec4  gamma = texture(uTexture0, vs_out.mTexCoordRect);
  float beta  = texture(uTexture1, vs_out.mTexCoordRect).r;
  vec3  alpha = gamma.rgb / max(gamma.a, 0.00001);
  vec3  background = texture(uTexture2, vs_out.mTexCoordScr).rgb;

  outColor.a    = 1.0f;
  outColor.rgb  = alpha * (1 - beta) + background * (beta);
}
)dy");

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy::builtin
{

FDyBtShOITIntegration::FDyBtShOITIntegration()
{
  this->mSpecifierName = sName;
  this->mVertexBuffer  = sVert;
  this->mPixelBuffer   = sFrag;
}

} /// ::dy::builtin namespace