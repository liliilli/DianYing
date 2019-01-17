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

in VS_OUT { vec2 texCoord; } fs_in;

layout (location = 0) out vec4 outColor;

layout (binding = 0) uniform sampler2D uTexture0; // Opaque
layout (binding = 1) uniform sampler2D uTexture1; // Color accumulation
layout (binding = 2) uniform sampler2D uTexture2; // Weight accumulation

vec3 GetOpaqueRgb() { return texture(uTexture0, fs_in.texCoord).rgb; }

void main()
{
  vec4  gamma = texture(uTexture1, fs_in.texCoord);
  float beta  = texture(uTexture2, fs_in.texCoord).r;
  vec3  alpha = gamma.rgb / max(gamma.a, 0.00001);

  outColor.a    = 1.0f;
  outColor.rgb  = alpha * (1 - beta) + GetOpaqueRgb() * (beta);
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