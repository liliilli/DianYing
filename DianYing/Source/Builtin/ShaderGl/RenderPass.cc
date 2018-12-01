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
#include <Dy/Builtin/ShaderGl/RenderPass.h>

namespace
{

constexpr std::string_view vertexShaderCode = R"dy(
#version 430 core

// Quad
layout (location = 0) in vec3 dyPosition;
layout (location = 1) in vec2 dyTexCoord0;

out gl_PerVertex { vec4 gl_Position; };
out VS_OUT { vec2 texCoord; } vs_out;

void main() {
	vs_out.texCoord		= dyTexCoord0;
    gl_Position			= vec4(dyPosition, 1.0);
}
)dy";

constexpr std::string_view fragmentShaderCode = R"dy(
#version 430

in VS_OUT { vec2 texCoord; } fs_in;
layout (location = 0) out vec4 outColor;

uniform sampler2D uUnlit;
uniform sampler2D uNormal;
uniform sampler2D uSpecular;
uniform sampler2D uViewPosition;

vec3 dirLight		= normalize(vec3(-1, 1, 0));
vec3 ambientColor	= vec3(1);

void main() {

	vec4 normalValue	= (texture(uNormal, fs_in.texCoord) - 0.5f) * 2.0f;
	vec4 unlitValue		= texture(uUnlit, fs_in.texCoord);

	float ambientFactor = 0.1f;
	float diffuseFactor = max(dot(normalValue.xyz, dirLight), 0.1);

    outColor = vec4(
		vec3(1) * diffuseFactor +
		ambientColor * ambientFactor,
		1.0f);
}
)dy";

} /// ::unnamed namespace

namespace dy::builtin
{

FDyBuiltinShaderGLRenderPass::FDyBuiltinShaderGLRenderPass()
{
  this->mSpecifierName  = sName;
  this->mVertexBuffer   = vertexShaderCode;
  this->mPixelBuffer    = fragmentShaderCode;
}

} /// ::dy::builtin namespace