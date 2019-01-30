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
#include <Dy/Builtin/ShaderGl/Render/BtShUvWorldPos.h>

//!
//! Forward declaration
//!

namespace
{

MDY_SET_IMMUTABLE_STRING(sVertexShaderCode, R"dy(
#version 430
#import <Input_DefaultVao>;
#import <Input_UboCamera>;
uniform mat4 modelMatrix;

out gl_PerVertex { vec4 gl_Position; };
out VS_OUT
{
	vec3 normal;
	vec4 modelPosition;
} vs_out;

void main()
{
  gl_Position			      = uCamera.mProjMatrix * uCamera.mViewMatrix * modelMatrix * vec4(dyPosition, 1.0);
	vs_out.normal		      = mat3(modelMatrix) * dyNormal;
	vs_out.modelPosition  = (modelMatrix * vec4(dyPosition, 1.0));
}
)dy");

MDY_SET_IMMUTABLE_STRING(sFragmentShaderCode, R"dy(
#version 430
#import <Input_DefaultTexture2D>;
#import <Output_OpaqueStream>;

in VS_OUT {
	vec3 normal;
	vec4 modelPosition;
} fs_in;

void main() {
  vec2 ts = fs_in.modelPosition.xz / 25.0f;
  ts.y    += fs_in.modelPosition.y / 25.0f;

	gUnlit	  = vec4(texture(uTexture0, ts).rgb, 1.0f);
	gNormal	  = vec4(normalize(fs_in.normal) * 0.5f + 0.5f, 1.0f);
	gSpecular = vec4(1, 0, 1, 1);
	gPosition = fs_in.modelPosition;
}
)dy");

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy::builtin
{

FDyBtShGlUvWorldPos::FDyBtShGlUvWorldPos()
{
  this->mSpecifierName  = sName;
  this->mVertexBuffer   = sVertexShaderCode;
  this->mPixelBuffer    = sFragmentShaderCode;
}

} /// ::dy::builtin namespace