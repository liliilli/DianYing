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
#include <Dy/Builtin/ShaderGl/Render/RenderOpaqueStatic.h>

//!
//! Forward declaration
//!

namespace
{

MDY_SET_IMMUTABLE_STRING(sVertexShaderCode, R"dy(
#version 430
#import <Input_DefaultVao>; 
#import <Input_UboCamera>;
#import <Input_ModelTransform>;

out gl_PerVertex { vec4 gl_Position; };
out VS_OUT
{
	vec3 fragColor;
	vec3 normal;
	vec2 texCoord;
	vec4 modelPosition;
} vs_out;

void main()
{
  gl_Position			      = DyGetCameraPv() * DyTransform(vec4(dyPosition, 1.0));
  vs_out.fragColor	    = dyNormal * 0.5f + 0.5f;
	vs_out.normal		      = DyGetRotationMatrix() * dyNormal;
	vs_out.texCoord		    = dyTexCoord0;
	vs_out.modelPosition  = DyTransform(vec4(dyPosition, 1.0));
}
)dy");

MDY_SET_IMMUTABLE_STRING(sFragmentShaderCode, R"dy(
#version 430
#import <Input_DefaultTexture2D>;
#import <Output_OpaqueStream>;

in VS_OUT {
	vec3 fragColor;
	vec3 normal;
	vec2 texCoord;
	vec4 modelPosition;
} fs_in;

void main() {
  DyBindActorId();
	gUnlit	  = vec4(texture(uTexture0, fs_in.texCoord).rgb, 1.0f);
	gNormal	  = vec4(normalize(fs_in.normal) * 0.5f + 0.5f, 1.0f);
	gSpecular = vec4(1, 0, 1, 1);
	gPosition = fs_in.modelPosition;
  DyNotSetEmissive();
}
)dy");

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy::builtin
{

FDyBuiltinShaderGLRenderOpaqueStatic::FDyBuiltinShaderGLRenderOpaqueStatic()
{
  this->mSpecifierName  = sName;
  this->mVertexBuffer   = sVertexShaderCode;
  this->mPixelBuffer    = sFragmentShaderCode;
}

} /// ::dy::builtin namespace