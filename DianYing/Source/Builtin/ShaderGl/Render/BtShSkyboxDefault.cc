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
#include <Dy/Builtin/ShaderGl/Render/BtShSkyboxDefault.h>

//!
//! Forward declaration
//!

namespace
{

MDY_SET_IMMUTABLE_STRING(sVertexShaderCode, R"dy(
#version 430
#import <Input_DefaultVao>;
#import <Input_UboCamera>;

mat4 DyGetCameraPV() { return uCamera.mProjMatrix * uCamera.mViewMatrix; }

out gl_PerVertex { vec4 gl_Position; };
out vec3 texCoord;

void main()
{
  gl_Position	= DyGetCameraPV() * vec4(dyPosition, 1.0);
  texCoord    = dyPosition;
}
)dy");

MDY_SET_IMMUTABLE_STRING(sFragmentShaderCode, R"dy(
#version 430
in vec3  texCoord;
out vec4 outFragColor;

uniform  samplerCube uTexture0; // Skybox;

void main() { outFragColor = texture(uTexture0, texCoord); }
)dy");

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy::builtin
{

FDyBtShSkyboxDefault::FDyBtShSkyboxDefault()
{
  this->mSpecifierName = sName;
  this->mVertexBuffer  = sVertexShaderCode;
  this->mPixelBuffer   = sFragmentShaderCode;
}

} /// ::dy::builtin namespac