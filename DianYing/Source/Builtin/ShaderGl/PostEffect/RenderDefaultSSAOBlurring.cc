#include <precompiled.h>
///
/// MIT License
/// Copyright (c) 2018 Jongmin Yun
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
#include <Dy/Builtin/ShaderGl/PostEffect/RenderDefaultSSAOBlurring.h>

//!
//! Forward declaration
//!

namespace
{

MDY_SET_IMMUTABLE_STRING(sVertexShaderCode, R"dy(
#version 430

layout (location = 0) in vec3 dyPosition;
layout (location = 1) in vec2 dyTexCoord0;

out gl_PerVertex { vec4 gl_Position; };
out VS_OUT { vec2 texCoord; } vs_out;

void main() {
	vs_out.texCoord		= dyTexCoord0;
    gl_Position			= vec4(dyPosition, 1.0);
}
)dy");

MDY_SET_IMMUTABLE_STRING(sPixelShaderCode, R"dy(
#version 430 core

out float FragColor;
in VS_OUT { vec2 texCoord; } fsIn;
uniform sampler2D uSsaoInput;

void main()
{
  vec2 texelSize = 1.0 / vec2(textureSize(uSsaoInput, 0));

  float result = 0.0;

  for (int x = -2; x < 2; ++x)
  {
    for (int y = -2; y < 2; ++y)
    {
      vec2 offset = vec2(float(x), float(y)) * texelSize;
      result += texture(uSsaoInput, fsIn.texCoord + offset).r;
    }
  }

  FragColor = result / (4.0 * 4.0);
}
)dy");

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy::builtin
{

FDyBuiltinShaderGLRenderDefaultSSAOBlurring::FDyBuiltinShaderGLRenderDefaultSSAOBlurring()
{
  this->mSpecifierName  = sName;
  this->mVertexBuffer   = sVertexShaderCode;
  this->mPixelBuffer    = sPixelShaderCode;
}

} /// ::dy::builtin namespace