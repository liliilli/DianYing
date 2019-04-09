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
#include <Dy/Builtin/ShaderGl/Debug/BtShDebugLine.h>

//!
//! Forward declaration
//!

namespace
{
  
MDY_SET_IMMUTABLE_STRING(sVertexShaderCode, R"dy(
#version 430
#import <Input_UboCamera>;

layout (location = 0) in vec3   dyPosition;

uniform mat4  uTransform;
uniform int   uColorIndex;

out gl_PerVertex { vec4 gl_Position; };
out VS_OUT { vec3 mColor; } vs_out;

vec3 colors[2] = { vec3(1, 1, 0), vec3(1, 0, 1) };

void main() {
  gl_Position   = uCamera.mProjMatrix * uCamera.mViewMatrix * uTransform * vec4(dyPosition, 1.0);
  vs_out.mColor = colors[uColorIndex];
}
)dy");

MDY_SET_IMMUTABLE_STRING(sFragmentShaderCode, R"dy(
#version 430

in VS_OUT { vec3 mColor; } fs_in;
layout (location = 0) out vec4 outColor;

void main()
{
  outColor = vec4(fs_in.mColor.rgb, 1.0f);
}
)dy");

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy::builtin
{

FDyBtShDebugLine::FDyBtShDebugLine()
{
  this->mSpecifierName  = sName;
  this->mVertexBuffer   = sVertexShaderCode;
  this->mPixelBuffer    = sFragmentShaderCode;
}

} /// ::dy::builtin namespace