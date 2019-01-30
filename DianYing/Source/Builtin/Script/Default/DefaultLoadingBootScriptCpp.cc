#include <precompiled.h>
///
/// MIT License
/// Copyright (c) 2018-2019 Jongmin Yun
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.
///

/// Header file
#include <Dy/Builtin/Script/Default/DefaultLoadingBootScriptCpp.h>
#include <Dy/Builtin/Texture/Checker.h>
#include <Dy/Builtin/Texture/ErrorBlue.h>

#include <Dy/Builtin/Mesh/Widget/FDyBtMsUiBarQuad.h>
#include <Dy/Builtin/Mesh/FDyBtMsUiImgQuad.h>

#include <Dy/Builtin/Model/Box.h>
#include <Dy/Builtin/Model/Plain.h>
#include <Dy/Builtin/Model/Sphere.h>
#include <Dy/Builtin/Model/ScreenProjectionTriangle.h>
#include <Dy/Builtin/Model/UIFontQuad.h>

#include <Dy/Builtin/ShaderGl/RenderColorGeometry.h>
#include <Dy/Builtin/ShaderGl/Render/RenderOpaqueStatic.h>
#include <Dy/Builtin/ShaderGl/RenderPass.h>
#include <Dy/Builtin/ShaderGl/RenderScreenOutput.h>
#include <Dy/Builtin/ShaderGl/Font/RenderDefaultFont.h>
#include <Dy/Builtin/ShaderGl/Font/RenderFontArraySDF.h>
#include <Dy/Builtin/ShaderGl/UI/RenderUIBasicGaugeBar.h>
#include <Dy/Builtin/ShaderGl/UI/RenderUIImage.h>
#include <Dy/Builtin/ShaderGl/PostEffect/RenderDefaultSSAO.h>
#include <Dy/Builtin/ShaderGl/PostEffect/RenderDefaultSSAOBlurring.h>
#include <Dy/Builtin/ShaderGl/Level/RenderCSMLightNormal.h>
#include <Dy/Builtin/ShaderGl/Level/RendIntegrationLevelCSM.h>

#include <Dy/Builtin/Material/OpaqueStaticPlain.h>

#include <Dy/Builtin/Attachment/CSM/FDyBtAtLight.h>

#include <Dy/Builtin/FrameBuffer/Deferred/FDyBtFrameBufferDeferred.h>
#include <Dy/Builtin/FrameBuffer/BasicShadow/FDyBtFBBasicShadow.h>
#include <Dy/Builtin/FrameBuffer/SceneIntegration/FDyBtFbOpaqueIntegration.h>
#include <Dy/Builtin/FrameBuffer/UIIntegration/FDyBtFbUiBasic.h>
#include <Dy/Builtin/FrameBuffer/WeightBlendOIT/FDyBtFbWeightBlendedOIT.h>
#include <Dy/Builtin/FrameBuffer/CSM/FDyBtFbCSM.h>
#include "Dy/Builtin/Texture/SSAONoiseMap.h"

namespace dy
{

FDyDefaultLoadingBootScript::FDyDefaultLoadingBootScript()
{
  using namespace builtin;
  using namespace dy;
  using namespace std::string_view_literals;

  MDY_LOADING_RESOURCE_BIND(EDyResourceType::Texture, FDyBuiltinTextureChecker::sName);
  MDY_LOADING_RESOURCE_BIND(EDyResourceType::Texture, FDyBuiltinTextureErrorBlue::sName);
  MDY_LOADING_RESOURCE_BIND(EDyResourceType::Texture, builtin::FDyBtTexSSAONoiseMap::sName);

  MDY_LOADING_RESOURCE_BIND(EDyResourceType::GLShader, FDyBuiltinShaderGLRenderColorGeometry::sName);
  MDY_LOADING_RESOURCE_BIND(EDyResourceType::GLShader, FDyBuiltinShaderGLRenderDefaultFont::sName);
  MDY_LOADING_RESOURCE_BIND(EDyResourceType::GLShader, FDyBuiltinShaderGLRenderDefaultSSAO::sName);
  MDY_LOADING_RESOURCE_BIND(EDyResourceType::GLShader, FDyBuiltinShaderGLRenderDefaultSSAOBlurring::sName);
  MDY_LOADING_RESOURCE_BIND(EDyResourceType::GLShader, FDyBuiltinShaderGLRenderFontArraySDF::sName);
  MDY_LOADING_RESOURCE_BIND(EDyResourceType::GLShader, FDyBuiltinShaderGLRenderOpaqueStatic::sName);
  MDY_LOADING_RESOURCE_BIND(EDyResourceType::GLShader, FDyBuiltinShaderGLRenderPass::sName);
  MDY_LOADING_RESOURCE_BIND(EDyResourceType::GLShader, FDyBuiltinShaderGLRenderScreenOutput::sName);
  MDY_LOADING_RESOURCE_BIND(EDyResourceType::GLShader, FDyBuiltinShaderGLRenderUiBasicGaugeBar::sName);
  MDY_LOADING_RESOURCE_BIND(EDyResourceType::GLShader, FDyBuiltinShaderGLRenderUiImage::sName);
  MDY_LOADING_RESOURCE_BIND(EDyResourceType::GLShader, FDyBtGlslRenderCSMLightNormal::sName);
  MDY_LOADING_RESOURCE_BIND(EDyResourceType::GLShader, FDyBtGlslRenderIntegrationLevelCSM::sName);
  MDY_LOADING_RESOURCE_BIND(EDyResourceType::GLShader, "dyBtShOITAccumulation"sv);
  MDY_LOADING_RESOURCE_BIND(EDyResourceType::GLShader, "dyBtShOITIntegration"sv);
  MDY_LOADING_RESOURCE_BIND(EDyResourceType::GLShader, "dyBtShGlOpaqueDynamic"sv);

  MDY_LOADING_RESOURCE_BIND(EDyResourceType::GLShader, FDyBtGlslRenderCSMLightNormal::sName);

  MDY_LOADING_RESOURCE_BIND(EDyResourceType::Model, FDyBuiltinModelBox::sName);
  MDY_LOADING_RESOURCE_BIND(EDyResourceType::Model, FDyBuiltinModelPlain::sName);
  MDY_LOADING_RESOURCE_BIND(EDyResourceType::Model, FDyBuiltinModelScreenProjectionTriangle::sName);
  MDY_LOADING_RESOURCE_BIND(EDyResourceType::Model, FDyBuiltinModelSphere::sName);
  MDY_LOADING_RESOURCE_BIND(EDyResourceType::Model, FDyBtMdUIFontQuad::sName);

  MDY_LOADING_RESOURCE_BIND(EDyResourceType::Mesh, FDyBtMsUiBarQuad::sName);
  MDY_LOADING_RESOURCE_BIND(EDyResourceType::Mesh, FDyBtMsUiImageQuad::sName);
  MDY_LOADING_RESOURCE_BIND(EDyResourceType::Mesh, "dyBtMsScrOITTriangle"sv);

  MDY_LOADING_RESOURCE_BIND(EDyResourceType::GLFrameBuffer, FDyBtFrameBufferDeferred::sName);
  //MDY_LOADING_RESOURCE_BIND(EDyResourceType::GLFrameBuffer, FDyBtFBBasicShadow::sName);
  MDY_LOADING_RESOURCE_BIND(EDyResourceType::GLFrameBuffer, builtin::FDyBtFbCSM::sName);
  MDY_LOADING_RESOURCE_BIND(EDyResourceType::GLFrameBuffer, FDyBtFbUiBasic::sName);
  MDY_LOADING_RESOURCE_BIND(EDyResourceType::GLFrameBuffer, builtin::FDyBtFbWeightBlendedOIT::sName);
  MDY_LOADING_RESOURCE_BIND(EDyResourceType::GLFrameBuffer, "dyBtFbWBOIT"sv);
  MDY_LOADING_RESOURCE_BIND(EDyResourceType::GLFrameBuffer, FDyBtFbOpaqueIntegration::sName);
  MDY_LOADING_RESOURCE_BIND(EDyResourceType::GLFrameBuffer, "dyBtFbIntgLevelTrans"sv);
  MDY_LOADING_RESOURCE_BIND(EDyResourceType::GLFrameBuffer, "dyBtFbSSAO"sv);
  MDY_LOADING_RESOURCE_BIND(EDyResourceType::GLFrameBuffer, "dyBtFbSSAOBlur"sv);

  MDY_LOADING_RESOURCE_BIND(EDyResourceType::GLAttachment,  "dyBtAtOpaqueOutput"sv);
  MDY_LOADING_RESOURCE_BIND(EDyResourceType::GLAttachment,  "dyBtAtWBOITColor"sv);
  MDY_LOADING_RESOURCE_BIND(EDyResourceType::GLAttachment,  "dyBtAtWBOITWeight"sv);
  MDY_LOADING_RESOURCE_BIND(EDyResourceType::GLAttachment,  "dyBtAtSSAOOutput"sv);
  MDY_LOADING_RESOURCE_BIND(EDyResourceType::GLAttachment,  "dyBtAtSSAOOutputFinal"sv);
}

} /// ::dy namespace