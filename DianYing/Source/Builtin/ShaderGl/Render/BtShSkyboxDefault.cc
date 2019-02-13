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
out vec2 screenUv;

uniform float uRotationDegree;

float DyDegToRad(float degree) { return degree * 3.1415926535 / 180.0; };

void main()
{
  // Make rotation (y) matrix.
  float yRad = DyDegToRad(uRotationDegree);
  mat4 rotationMatrix = mat4(
    vec4(cos(yRad), 0, sin(yRad), 0),
    vec4(0, 1, 0, 0),
    vec4(-sin(yRad), 0, cos(yRad), 0),
    vec4(0, 0, 0, 1)
  );

  // Set aligned view.
  mat4 alignedViewMatrix = uCamera.mViewMatrix;
  alignedViewMatrix[3] = vec4(0, 0, 0, 1);
  gl_Position	= uCamera.mProjMatrix * alignedViewMatrix * rotationMatrix * vec4(dyPosition * 10000.f, 1.0);
  gl_Position = gl_Position.xyww;
//.z = min(gl_Position.z, gl_Position.w);

  texCoord    = dyPosition * -1.0f;
  screenUv    = (((gl_Position / gl_Position.w) + 1.0f) * 0.5f).xy;
}
)dy");

MDY_SET_IMMUTABLE_STRING(sFragmentShaderCode, R"dy(
#version 430
in vec3   texCoord;
in vec2   screenUv;
out vec4  outFragColor;

uniform  samplerCube  uTexture0; // Skybox;
uniform  sampler2D    uTexture1; // Unlit
uniform  float        uExposure;
uniform  vec3         uTintColor;

void main() 
{ 
  //if (texture(uTexture1, screenUv).a >= 0.2f) { discard; }
  vec4 color = texture(uTexture0, texCoord);
  outFragColor.rgb  = color.rgb * uTintColor * uExposure;
  outFragColor.a    = color.a;
}
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