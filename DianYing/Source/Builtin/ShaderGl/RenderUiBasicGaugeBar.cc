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
#include <Dy/Builtin/ShaderGl/RenderUIBasicGaugeBar.h>
#include <Dy/Core/Component/Internal/ShaderType.h>
#include <Dy/Management/IO/IODataManager.h>
#include <Dy/Management/IO/IOResourceManager.h>

//!
//! Forward declaration
//!

namespace
{

MDY_SET_IMMUTABLE_STRING(sVertexShaderCode, R"dy(
#version 430 core

layout (location = 0) in vec2 dyPosition;
uniform mat4 uUiProjMatrix;

void main() {
	gl_Position = vec4((uUiProjMatrix * vec4(dyPosition, 0, 1)).xy, 0, 1);
}
)dy");

MDY_SET_IMMUTABLE_STRING(sFragmentShaderCode, R"dy(
#version 430 core

uniform vec4 uFillColor;
layout (location = 0) out vec4 gOutput;

void main() { gOutput = uFillColor; }
)dy");

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy::builtin
{

FDyBuiltinShaderGLRenderUiBasicGaugeBar::FDyBuiltinShaderGLRenderUiBasicGaugeBar()
{
  PDyShaderConstructionDescriptor shaderDesc;
  shaderDesc.mShaderName = sName;
  {
    PDyShaderFragmentInformation vs;
    vs.mShaderType = EDyShaderFragmentType::Vertex;
    vs.mShaderRawCode = sVertexShaderCode;
    vs.mIsEnabledRawLoadShaderCode = true;
    shaderDesc.mShaderFragments.emplace_back(vs);
  }
  {
    PDyShaderFragmentInformation fs;
    fs.mShaderType = EDyShaderFragmentType::Pixel;
    fs.mShaderRawCode = sFragmentShaderCode;
    fs.mIsEnabledRawLoadShaderCode = true;
    shaderDesc.mShaderFragments.emplace_back(fs);
  }

  auto& infoManager = MDyIOData::GetInstance();
  auto& rescManager = MDyIOResource::GetInstance();

  MDY_CALL_ASSERT_SUCCESS(infoManager.CreateShaderInformation(shaderDesc));
  MDY_CALL_ASSERT_SUCCESS(rescManager.CreateShaderResource(shaderDesc.mShaderName));
}

} /// ::dy namespace