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
#include <Dy/Builtin/ShaderGl/PostEffect/RenderDefaultSSAO.h>

//!
//! Forward declaration
//!

namespace
{

MDY_SET_IMMUTABLE_STRING(sVertexShaderCode, R"dy(
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
)dy");

MDY_SET_IMMUTABLE_STRING(sFragmentShaderCode, R"dy(
#version 430 core

layout (location = 0) out float FragColor;
in VS_OUT { vec2 texCoord; } fsIn;

uniform sampler2D ugPosition;
uniform sampler2D ugNormal;
uniform sampler2D uTextureNoise;
uniform vec3	  uSamples[64];
uniform float     uKernelSize	= 64;
uniform float     uRadius		= 1.5f;
uniform float     uBias			= 0.025f;
uniform vec2      uScreenSize   = vec2(1280, 720);
uniform mat4	  uProjection;

void main()
{
	const vec2 noiseScale = uScreenSize / 4.0f;

	// get input for SSAO algorithm
    vec3 fragPos	=			texture(ugPosition,		fsIn.texCoord).xyz;
    vec3 normal		= normalize(texture(ugNormal,		fsIn.texCoord).rgb);
    vec3 randomVec= normalize(texture(uTextureNoise,	fsIn.texCoord * noiseScale).xyz);

    // create TBN change-of-basis matrix: from tangent-space to view-space
    vec3 tangent	= normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent= cross(normal, tangent);
    mat3 TBN		  = mat3(tangent, bitangent, normal);

    // iterate over the sample kernel and calculate occlusion factor
    float occlusion = 0.0;

    for(int i = 0; i < uKernelSize; ++i)
    {
        // get sample position
        vec3 kernelSample	= TBN * uSamples[i]; // from tangent to view-space
        kernelSample		= fragPos + kernelSample * uRadius;

        // project sample position (to sample texture) (to get position on screen/texture)
        // from view to clip-space // perspective divide // transform to range 0.0 - 1.0
        vec4 offset = vec4(kernelSample, 1.0);
        offset		= uProjection * offset;
        offset.xyz /= offset.w;
        offset.xyz	= offset.xyz * 0.5 + 0.5;

        // get sample depth and range check & accumulate
        float sampleDepth	= texture(ugPosition, offset.xy).z; // get depth value of kernel sample
        float rangeCheck	= smoothstep(0.0, 1.0, uRadius / abs(fragPos.z - sampleDepth));
        occlusion		   += (sampleDepth >= kernelSample.z + uBias ? 1.0 : 0.0) * rangeCheck;
    }

    FragColor = 1.0 - (occlusion / uKernelSize);
}
)dy");

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy::builtin
{

FDyBuiltinShaderGLRenderDefaultSSAO::FDyBuiltinShaderGLRenderDefaultSSAO()
{
  this->mSpecifierName  = sName;
  this->mVertexBuffer   = sVertexShaderCode;
  this->mPixelBuffer    = sFragmentShaderCode;
}

} /// ::dy::builtin namespace