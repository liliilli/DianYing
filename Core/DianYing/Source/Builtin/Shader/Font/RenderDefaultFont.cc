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
#include <Dy/Builtin/ShaderGl/Font/RenderDefaultFont.h>

//!
//! Forward declaration
//!

namespace
{

MDY_SET_IMMUTABLE_STRING(sVertexShaderCode, R"dy(
#version 430 core

layout (location = 0) in vec2 dyPosition;
layout (location = 1) in vec2 dyTexCoord0;

uniform mat4 uUiProjMatrix;

out VS_OUT { vec2 texCoord; } vs_out;

void main() {
	gl_Position     = vec4((uUiProjMatrix * vec4(dyPosition, 0, 1)).xy, 0, 1);
	vs_out.texCoord = dyTexCoord0.xy;
}
)dy");

MDY_SET_IMMUTABLE_STRING(sFragmentShaderCode, R"dy(
#version 430 core

in VS_OUT { vec2 texCoord; } fs_in;

layout (binding = 0) uniform sampler2D uCharTexture;
uniform vec4 uTextColor;

layout (location = 0) out vec4 gOutput;

void main()
{
  //float alpha = texture(uCharTexture, fs_in.texCoord).r;
  //if (alpha <= 0.0f) { discard; }

	gOutput = uTextColor * vec4(vec3(1), texture(uCharTexture, fs_in.texCoord).r);
}
)dy");

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy
{

builtin::FDyBuiltinShaderGLRenderDefaultFont::FDyBuiltinShaderGLRenderDefaultFont()
{
  this->mSpecifierName  = sName;
  this->mVertexBuffer   = sVertexShaderCode;
  this->mPixelBuffer    = sFragmentShaderCode;
}

} /// ::dy namespace