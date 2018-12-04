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
#include <Dy/Builtin/ShaderGl/UI/RenderUIImage.h>

//!
//! Forward declaration
//!

namespace
{

MDY_SET_IMMUTABLE_STRING(sVertexShaderCode, R"dy(
#version 430 core

layout (location = 0) in vec2 dyPosition;
layout (location = 1) in vec2 dyTexCoord;
uniform mat4 uUiProjMatrix;
uniform mat3 uTransformMatrix;

out VS_OUT { vec2 texCoord; } vsOut;

void main() {
	gl_Position = vec4((uUiProjMatrix * vec4(dyPosition, 0, 1)).xy, 0, 1);
  vsOut.texCoord = dyTexCoord;
}
)dy");

MDY_SET_IMMUTABLE_STRING(sFragmentShaderCode, R"dy(
#version 430 core

uniform vec4 uTintColor;
layout (binding = 0)  uniform sampler2D uImageTexture;
layout (location = 0) out vec4 gOutput;

in VS_OUT { vec2 texCoord; } fsIn;

void main() {
  gOutput = texture(uImageTexture, fsIn.texCoord) * uTintColor;
}
)dy");

} /// ::unnamed namesapce

//!
//! Implementation
//!

namespace dy::builtin
{

FDyBuiltinShaderGLRenderUiImage::FDyBuiltinShaderGLRenderUiImage()
{
  this->mSpecifierName  = sName;
  this->mVertexBuffer   = sVertexShaderCode;
  this->mPixelBuffer    = sFragmentShaderCode;
}

} /// ::dy::builtin namespace