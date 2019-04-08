#pragma once
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

#include <Dy/Builtin/Attachment/BasicShadow/BasicShadow.h>
#include <Dy/Builtin/Attachment/CSM/FDyBtAtLight.h>
#include <Dy/Builtin/Attachment/Debug/FDyBtAtDebug.h>

#include <Dy/Builtin/Attachment/Deferred/FDyBtAtDepthZValue.h>
#include <Dy/Builtin/Attachment/Deferred/FDyBtAtEmissive.h>
#include <Dy/Builtin/Attachment/Deferred/FDyBtAtSkyOutput.h>
#include <Dy/Builtin/Attachment/Deferred/FDyBtAtSSAOFinalOutput.h>
#include <Dy/Builtin/Attachment/Deferred/FDyBtAtSSAOOutput.h>
#include <Dy/Builtin/Attachment/Deferred/FDyBtAttachmentNormal.h>
#include <Dy/Builtin/Attachment/Deferred/FDyBtAttachmentPosition.h>
#include <Dy/Builtin/Attachment/Deferred/FDyBtAttachmentSpecular.h>
#include <Dy/Builtin/Attachment/Deferred/FDyBtAttachmentUnlit.h>

#include <Dy/Builtin/Attachment/Emissive/FTempBtAtEmissiveBlur.h>
#include <Dy/Builtin/Attachment/Emissive/FTempBtAtEmissiveResult.h>

#include <Dy/Builtin/Attachment/SceneIntegration/FDyBtAtActorId.h>
#include <Dy/Builtin/Attachment/SceneIntegration/FDyBtAtOpaqueOutput.h>
#include <Dy/Builtin/Attachment/SceneIntegration/FDyBtAttOutput.h>

#include <Dy/Builtin/Attachment/UIIntegration/FDyBtAttUiBasic.h>
#include <Dy/Builtin/Attachment/UIIntegration/FDyBtAtUiId.h>

#include <Dy/Builtin/Attachment/WeightBlendOIT/FDyBtAtOITColor.h>
#include <Dy/Builtin/Attachment/WeightBlendOIT/FDyBtAtOITDepth.h>
#include <Dy/Builtin/Attachment/WeightBlendOIT/FDyBtAtOITWeight.h>

#include <Dy/Builtin/FrameBuffer/BasicShadow/FDyBtFBBasicShadow.h>
#include <Dy/Builtin/FrameBuffer/CSM/FDyBtFbCSM.h>
#include <Dy/Builtin/FrameBuffer/Debug/FDyBtFbDebug.h>

#include <Dy/Builtin/FrameBuffer/Deferred/FDyBtFbSkybox.h>
#include <Dy/Builtin/FrameBuffer/Deferred/FDyBtFbSSAO.h>
#include <Dy/Builtin/FrameBuffer/Deferred/FDyBtFbSSAOBlur.h>
#include <Dy/Builtin/FrameBuffer/Deferred/FDyBtFrameBufferDeferred.h>

#include <Dy/Builtin/FrameBuffer/Emissive/FDyBtFbEmissiveBlur.h>
#include <Dy/Builtin/FrameBuffer/Emissive/FDyBtFbEmissiveToneMap.h>

#include <Dy/Builtin/FrameBuffer/SceneIntegration/FDyBtFbOpaqueIntegration.h>
#include <Dy/Builtin/FrameBuffer/SceneIntegration/FDyBtFbTransIntegration.h>
#include <Dy/Builtin/FrameBuffer/UIIntegration/FDyBtFbUiBasic.h>
#include <Dy/Builtin/FrameBuffer/WeightBlendOIT/FDyBtFbWeightBlendedOIT.h>

#include <Dy/Builtin/GlobalInstance/FDyBtGiDebugStatus.h>
#include <Dy/Builtin/Material/BtMtCheckerWorldPos.h>
#include <Dy/Builtin/Material/OpaqueStaticPlain.h>

#include <Dy/Builtin/Material/Opaque/MatOpaqueEmissiveStatic.h>
#include <Dy/Builtin/Material/Transparent/StaticTransparentChecker.h>

#include <Dy/Builtin/Mesh/FDyBtMsBox.h>
#include <Dy/Builtin/Mesh/FDyBtMsPlain.h>
#include <Dy/Builtin/Mesh/FDyBtMsScrTriangle.h>
#include <Dy/Builtin/Mesh/FDyBtMsSphere.h>
#include <Dy/Builtin/Mesh/FDyBtMsUiFontQuad.h>
#include <Dy/Builtin/Mesh/FDyBtMsUiImgQuad.h>

#include <Dy/Builtin/Mesh/Debug/FDyBtMsDebugAABB.h>
#include <Dy/Builtin/Mesh/Debug/FDyBtMsDebugBox.h>
#include <Dy/Builtin/Mesh/Debug/FDyBtMsDebugCapsule.h>
#include <Dy/Builtin/Mesh/Debug/FDyBtMsDebugSphere.h>

//#include <Dy/Builtin/Mesh/Instancing/FDyBtMsInstancing.h>
#include <Dy/Builtin/Mesh/Plane/FDyBtMsOITTriangle.h>
#include <Dy/Builtin/Mesh/Widget/FDyBtMsUiBarQuad.h>

#include <Dy/Builtin/Model/Box.h>
#include <Dy/Builtin/Model/BoxOIT.h>
#include <Dy/Builtin/Model/EmissiveSphere.h>
#include <Dy/Builtin/Model/Plain.h>
#include <Dy/Builtin/Model/ScreenProjectionTriangle.h>
#include <Dy/Builtin/Model/Sphere.h>
#include <Dy/Builtin/Model/UIFontQuad.h>

#include <Dy/Builtin/Prefab/FBtPbCameraMan.h>
#include <Dy/Builtin/Prefab/FBtPbDefaultSphere.h>

#include <Dy/Builtin/RenderItem/FBtLevelIntegeration.h>
#include <Dy/Builtin/RenderItem/FBtScreenIntegration.h>

#include <Dy/Builtin/RenderItem/Debug/FBtDebugCollisionAabb.h>
#include <Dy/Builtin/RenderItem/Debug/FBtDebugCollisionShape.h>
#include <Dy/Builtin/RenderItem/Debug/FBtDebugPicking.h>

#include <Dy/Builtin/RenderItem/Level/FBtLevelOITDefault.h>
#include <Dy/Builtin/RenderItem/Level/FBtOpaqueDefault.h>
#include <Dy/Builtin/RenderItem/Level/FBtPpCsmShadow.h>
#include <Dy/Builtin/RenderItem/Level/FBtPpEmissive.h>
#include <Dy/Builtin/RenderItem/Level/FBtPpSkybox.h>
#include <Dy/Builtin/RenderItem/Level/FBtPpSsao.h>

#include <Dy/Builtin/RenderItem/Widget/FBtWidgetGeneral.h>

#include <Dy/Builtin/RenderPipeline/FBtDefault.h>
#include <Dy/Builtin/RenderPipeline/FBtDefaultLoading.h>

#include <Dy/Builtin/RenderPipeline/Debug/FBtDebug.h>
#include <Dy/Builtin/RenderPipeline/Widget/FBtWidget.h>
#include <Dy/Builtin/RenderPipeline/Level/FBtDefaultLevel.h>
#include <Dy/Builtin/RenderPipeline/Level/FBtDefaultLevelPostProcess.h>
#include <Dy/Builtin/RenderPipeline/Level/FBtDefaultLevelRender.h>

#include <Dy/Builtin/Script/BtScrGlobalDefaultUniformBuffer.h>
#include <Dy/Builtin/Script/DebugUiScriptCpp.h>
#include <Dy/Builtin/Script/Default/DefaultLoadingBootScriptCpp.h>
#include <Dy/Builtin/Script/Default/DefaultLoadingGlobalScriptCpp.h>

#include <Dy/Builtin/ShaderGl/RenderColorGeometry.h>
#include <Dy/Builtin/ShaderGl/RenderPass.h>
#include <Dy/Builtin/ShaderGl/RenderScreenOutput.h>

#include <Dy/Builtin/ShaderGl/Debug/BtShDebugLine.h>
#include <Dy/Builtin/ShaderGl/Editor/RenderGrid.h>
#include <Dy/Builtin/ShaderGl/Font/RenderDefaultFont.h>
#include <Dy/Builtin/ShaderGl/Font/RenderFontArraySDF.h>

#include <Dy/Builtin/ShaderGl/Level/BtShOITAccumulation.h>
#include <Dy/Builtin/ShaderGl/Level/BtShOITIntegration.h>
#include <Dy/Builtin/ShaderGl/Level/RenderCSMInstancing.h>
#include <Dy/Builtin/ShaderGl/Level/RenderCSMLightNormal.h>
#include <Dy/Builtin/ShaderGl/Level/RendIntegrationLevelCSM.h>

#include <Dy/Builtin/ShaderGl/PostEffect/RenderDefaultSSAO.h>
#include <Dy/Builtin/ShaderGl/PostEffect/RenderDefaultSSAOBlurring.h>

#include <Dy/Builtin/ShaderGl/PostEffect/Emissive/BtShEmissiveBlur.h>
#include <Dy/Builtin/ShaderGl/PostEffect/Emissive/BtShEmissiveToneMap.h>

#include <Dy/Builtin/ShaderGl/Render/BtShSkyboxDefault.h>
#include <Dy/Builtin/ShaderGl/Render/BtShUvWorldPos.h>
#include <Dy/Builtin/ShaderGl/Render/FDyBtShOITPlainColor.h>
#include <Dy/Builtin/ShaderGl/Render/OpaqueEmissiveStatic.h>
#include <Dy/Builtin/ShaderGl/Render/RenderOpaqueDynamic.h>
#include <Dy/Builtin/ShaderGl/Render/RenderOpaqueStatic.h>

#include <Dy/Builtin/Texture/Checker.h>
#include <Dy/Builtin/Texture/CheckerBright.h>
#include <Dy/Builtin/Texture/ErrorBlue.h>
#include <Dy/Builtin/Texture/FDyBtTexWhiteTexture.h>
#include <Dy/Builtin/Texture/SSAONoiseMap.h>

#include <Dy/Builtin/Widget/DebugUiMeta.h>
#include <Dy/Builtin/Widget/FDyBtWdTestLoadingUi.h>
