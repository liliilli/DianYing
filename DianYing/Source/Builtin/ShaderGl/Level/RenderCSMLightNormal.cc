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
#include <Dy/Builtin/ShaderGl/Level/RenderCSMLightNormal.h>

//!
//! Forward declaration and data
//!

namespace
{

MDY_SET_IMMUTABLE_STRING(sVert, R"dy(
#version 430

layout (location = 0) in vec3 dyPosition;

uniform mat4 uPvLightMatrix;
uniform mat4 uModelMatrix;

void main() {
    gl_Position = uPvLightMatrix * uModelMatrix * vec4(dyPosition, 1.0);
}
)dy");

MDY_SET_IMMUTABLE_STRING(sGeom, R"dy(
#version 430

layout(triangles) in;
layout(triangle_strip, max_vertices = 12) out;
uniform int uFrustumSegmentCount;

void main(void)
{
  for (int segment = 0; segment < uFrustumSegmentCount; ++segment)
  {
    for (int i = 0; i < gl_in.length(); ++i)
    {
      gl_Position = gl_in[i].gl_Position;
      gl_ViewportIndex = segment;
      gl_Layer = segment;
      EmitVertex();
    }
    EndPrimitive();
  }
}

)dy");

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy::builtin
{

FDyBtGlslRenderCSMLightNormal::FDyBtGlslRenderCSMLightNormal()
{
  this->mSpecifierName  = sName;
  this->mVertexBuffer   = sVert;
  this->mGeometryBuffer = sGeom;
}

} /// ::dy::builtin namespace