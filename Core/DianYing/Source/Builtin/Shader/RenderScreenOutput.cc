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
#include <Dy/Builtin/ShaderGl/RenderScreenOutput.h>

//!
//! Forward declaration
//!

namespace
{

MDY_SET_IMMUTABLE_STRING(sVertexShaderCode, R"dy(
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

MDY_SET_IMMUTABLE_STRING(sFragmentShaderCode, R"dy(
#version 430

in VS_OUT { vec2 texCoord; } fs_in;
layout (location = 0) out vec4 outColor;

layout (binding = 0) uniform sampler2D uTexture0; // uSceneTexture;
layout (binding = 1) uniform sampler2D uTexture1; // uUiTexture;
layout (binding = 2) uniform sampler2D uTexture2; // uDebugTexture;

void main()
{
  const vec4 sc = texture(uTexture0, fs_in.texCoord);
  const vec4 ui = texture(uTexture1, fs_in.texCoord);
  const vec4 db = texture(uTexture2, fs_in.texCoord);

  outColor = vec4(
      (vec3(sc.rgb) * (1 - ui.a))
    + (vec3(ui.rgb) * (ui.a)),
    1.0f);
  if (db.a == 1.0f) { outColor = db; }
}
)dy");

} /// ::unnamed namespace

//!
//! Implmentation
//!

namespace dy::builtin
{

FDyBuiltinShaderGLRenderScreenOutput::FDyBuiltinShaderGLRenderScreenOutput()
{
  this->mSpecifierName  = sName;
  this->mVertexBuffer   = sVertexShaderCode;
  this->mPixelBuffer    = sFragmentShaderCode;
}

} /// ::dy::builtin namespace