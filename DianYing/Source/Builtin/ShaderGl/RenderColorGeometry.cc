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
#include <Dy/Builtin/ShaderGl/RenderColorGeometry.h>
#include <Dy/Core/Component/Internal/ShaderType.h>
#include <Dy/Management/DataInformationManager.h>
#include <Dy/Management/HeapResourceManager.h>

//!
//! Local immutable data
//!

namespace
{

[[maybe_unused]]
MDY_SET_IMMUTABLE_STRING(sVertexShaderCode, R"dy(
#version 430 core

layout (location = 0) in vec3 dyPosition;
out gl_PerVertex { vec4 gl_Position; };

void main()
{
  gl_Position	    = vec4(dyPosition, 1.0);
}
)dy");

[[maybe_unused]]
MDY_SET_IMMUTABLE_STRING(sFragmentShaderCode, R"dy(
#version 430

layout (location = 0) out vec4 outColor;
uniform vec3 uColor;

void main()
{
  outColor = vec4(uColor, 1.0f);
}
)dy");

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy::builtin
{

FDyBuiltinShaderGLRenderColorGeometry::FDyBuiltinShaderGLRenderColorGeometry()
{
  PDyShaderConstructionDescriptor shaderDesc;
  shaderDesc.mShaderName = FDyBuiltinShaderGLRenderColorGeometry::sName;
  {
    PDyShaderFragmentInformation vs;
    vs.mShaderType                  = EDyShaderFragmentType::Vertex;
    vs.mShaderRawCode               = sVertexShaderCode;
    vs.mIsEnabledRawLoadShaderCode  = true;
    shaderDesc.mShaderFragments.emplace_back(vs);
  }
  {
    PDyShaderFragmentInformation fs;
    fs.mShaderType                  = EDyShaderFragmentType::Pixel;
    fs.mShaderRawCode               = sFragmentShaderCode;
    fs.mIsEnabledRawLoadShaderCode  = true;
    shaderDesc.mShaderFragments.emplace_back(fs);
  }

  auto& infoManager = MDyDataInformation::GetInstance();
  auto& rescManager = MDyHeapResource::GetInstance();

  MDY_CALL_ASSERT_SUCCESS(infoManager.CreateShaderInformation(shaderDesc));
  MDY_CALL_ASSERT_SUCCESS(rescManager.CreateShaderResource(FDyBuiltinShaderGLRenderColorGeometry::sName.data()));
}

} /// ::dy::builtin namespace