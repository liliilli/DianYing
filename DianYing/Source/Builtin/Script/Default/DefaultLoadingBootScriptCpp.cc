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

  MDY_LOADING_RESOURCE_BIND(EResourceType::Texture, FDyBuiltinTextureChecker::sName);
  MDY_LOADING_RESOURCE_BIND(EResourceType::Texture, FDyBuiltinTextureErrorBlue::sName);
  MDY_LOADING_RESOURCE_BIND(EResourceType::Texture, builtin::FDyBtTexSSAONoiseMap::sName);

  MDY_LOADING_RESOURCE_BIND(EResourceType::GLShader, FDyBuiltinShaderGLRenderColorGeometry::sName);
  MDY_LOADING_RESOURCE_BIND(EResourceType::GLShader, FDyBuiltinShaderGLRenderDefaultFont::sName);
  MDY_LOADING_RESOURCE_BIND(EResourceType::GLShader, FDyBuiltinShaderGLRenderDefaultSSAO::sName);
  MDY_LOADING_RESOURCE_BIND(EResourceType::GLShader, FDyBuiltinShaderGLRenderDefaultSSAOBlurring::sName);
  MDY_LOADING_RESOURCE_BIND(EResourceType::GLShader, FDyBuiltinShaderGLRenderFontArraySDF::sName);
  MDY_LOADING_RESOURCE_BIND(EResourceType::GLShader, FDyBuiltinShaderGLRenderOpaqueStatic::sName);
  MDY_LOADING_RESOURCE_BIND(EResourceType::GLShader, FDyBuiltinShaderGLRenderPass::sName);
  MDY_LOADING_RESOURCE_BIND(EResourceType::GLShader, FDyBuiltinShaderGLRenderScreenOutput::sName);
  MDY_LOADING_RESOURCE_BIND(EResourceType::GLShader, FDyBuiltinShaderGLRenderUiBasicGaugeBar::sName);
  MDY_LOADING_RESOURCE_BIND(EResourceType::GLShader, FDyBuiltinShaderGLRenderUiImage::sName);
  MDY_LOADING_RESOURCE_BIND(EResourceType::GLShader, FDyBtGlslRenderCSMLightNormal::sName);
  MDY_LOADING_RESOURCE_BIND(EResourceType::GLShader, FDyBtGlslRenderIntegrationLevelCSM::sName);
  MDY_LOADING_RESOURCE_BIND(EResourceType::GLShader, "dyBtShOITAccumulation");
  MDY_LOADING_RESOURCE_BIND(EResourceType::GLShader, "dyBtShOITIntegration");
  MDY_LOADING_RESOURCE_BIND(EResourceType::GLShader, "dyBtShGlOpaqueDynamic");
  MDY_LOADING_RESOURCE_BIND(EResourceType::GLShader, "dyBtShDebugLine");
  MDY_LOADING_RESOURCE_BIND(EResourceType::GLShader, "dyBtShGlSkyboxDefault");

  MDY_LOADING_RESOURCE_BIND(EResourceType::GLShader, FDyBtGlslRenderCSMLightNormal::sName);

  MDY_LOADING_RESOURCE_BIND(EResourceType::Model, FDyBuiltinModelBox::sName);
  MDY_LOADING_RESOURCE_BIND(EResourceType::Model, FDyBuiltinModelPlain::sName);
  MDY_LOADING_RESOURCE_BIND(EResourceType::Model, FDyBuiltinModelScreenProjectionTriangle::sName);
  MDY_LOADING_RESOURCE_BIND(EResourceType::Model, FDyBuiltinModelSphere::sName);
  MDY_LOADING_RESOURCE_BIND(EResourceType::Model, FDyBtMdUIFontQuad::sName);

  MDY_LOADING_RESOURCE_BIND(EResourceType::Mesh, FDyBtMsUiBarQuad::sName);
  MDY_LOADING_RESOURCE_BIND(EResourceType::Mesh, FDyBtMsUiImageQuad::sName);
  MDY_LOADING_RESOURCE_BIND(EResourceType::Mesh, "dyBtMsScrOITTriangle");
  MDY_LOADING_RESOURCE_BIND(EResourceType::Mesh, "dyBtMsDebugSphere");
  MDY_LOADING_RESOURCE_BIND(EResourceType::Mesh, "dyBtMsDebugBox");
  MDY_LOADING_RESOURCE_BIND(EResourceType::Mesh, "dyBtMsDebugCapsule");
  MDY_LOADING_RESOURCE_BIND(EResourceType::Mesh, "dyBtMsDebugAABB");

  MDY_LOADING_RESOURCE_BIND(EResourceType::GLFrameBuffer, FDyBtFrameBufferDeferred::sName);
  //MDY_LOADING_RESOURCE_BIND(EResourceType::GLFrameBuffer, FDyBtFBBasicShadow::sName);
  MDY_LOADING_RESOURCE_BIND(EResourceType::GLFrameBuffer, builtin::FDyBtFbCSM::sName);
  MDY_LOADING_RESOURCE_BIND(EResourceType::GLFrameBuffer, FDyBtFbUiBasic::sName);
  MDY_LOADING_RESOURCE_BIND(EResourceType::GLFrameBuffer, builtin::FDyBtFbWeightBlendedOIT::sName);
  MDY_LOADING_RESOURCE_BIND(EResourceType::GLFrameBuffer, "dyBtFbWBOIT");
  MDY_LOADING_RESOURCE_BIND(EResourceType::GLFrameBuffer, FDyBtFbOpaqueIntegration::sName);
  MDY_LOADING_RESOURCE_BIND(EResourceType::GLFrameBuffer, "dyBtFbIntgLevelTrans");
  MDY_LOADING_RESOURCE_BIND(EResourceType::GLFrameBuffer, "dyBtFbSSAO");
  MDY_LOADING_RESOURCE_BIND(EResourceType::GLFrameBuffer, "dyBtFbSSAOBlur");
  MDY_LOADING_RESOURCE_BIND(EResourceType::GLFrameBuffer, "dyBtFbDebug");
  MDY_LOADING_RESOURCE_BIND(EResourceType::GLFrameBuffer, "dyBtFbSkybox");

  MDY_LOADING_RESOURCE_BIND(EResourceType::GLAttachment,  "dyBtAtOpaqueOutput");
  MDY_LOADING_RESOURCE_BIND(EResourceType::GLAttachment,  "dyBtAtWBOITColor");
  MDY_LOADING_RESOURCE_BIND(EResourceType::GLAttachment,  "dyBtAtWBOITWeight");
  MDY_LOADING_RESOURCE_BIND(EResourceType::GLAttachment,  "dyBtAtSSAOOutput");
  MDY_LOADING_RESOURCE_BIND(EResourceType::GLAttachment,  "dyBtAtSSAOOutputFinal");
  MDY_LOADING_RESOURCE_BIND(EResourceType::GLAttachment,  "dyBtAtUiId");
  MDY_LOADING_RESOURCE_BIND(EResourceType::GLAttachment,  "dyBtAtActorId");
}

} /// ::dy namespace