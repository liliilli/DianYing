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
#include <Dy/Builtin/ShaderGl/Render/RenderOpaqueDynamic.h>

//!
//! Forward declaration
//!

namespace
{

MDY_SET_IMMUTABLE_STRING(sVert, R"dy(
#version 430
#import <Input_DefaultVao>; 
#import <Input_SkinAnimation>;
#import <Input_UboCamera>;
uniform mat4 uModelMatrix;

out gl_PerVertex { vec4 gl_Position; };
out VS_OUT
{
	vec3 normal;
	vec2 texCoord;
	vec4 modelPosition;
} vs_out;

void main()
{
	vs_out.modelPosition  = uModelMatrix * Dy_GetBoneTransform() * vec4(dyPosition, 1.0);
	vs_out.normal		      = mat3(uModelMatrix) * mat3(Dy_GetBoneTransform()) * dyNormal;
	vs_out.texCoord		    = dyTexCoord0;

  gl_Position			      = Dy_GetCameraPv() * vs_out.modelPosition; 
}
)dy");

MDY_SET_IMMUTABLE_STRING(sFrag, R"dy(
#version 430
#import <Input_DefaultTexture2D>;
#import <Output_OpaqueStream>;

in VS_OUT {
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
}
)dy");

} /// ::anonymouse namespace

//!
//! Implementation
//!

namespace dy::builtin
{
  
FDyBuiltinShaderGLRenderOpaqueDynamic::FDyBuiltinShaderGLRenderOpaqueDynamic()
{
  this->mSpecifierName  = sName;
  this->mVertexBuffer   = sVert;
  this->mPixelBuffer    = sFrag;
}

} /// ::dy::builtin namespace