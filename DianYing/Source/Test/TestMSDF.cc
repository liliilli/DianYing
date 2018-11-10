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
#include <Dy/Test/TestMSDF.h>

#undef max
#undef min
#undef INFINITE
#if defined(_WIN32)
#pragma warning(push)
#pragma warning(disable:4244 4267 4458)
#endif

#include <msdfgen.h>
//#include <msdfgen-ext.h>
#if defined(_WIN32)
#pragma warning(pop)
#endif

#include <glm/gtc/matrix_transform.hpp>

#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftoutln.h>
#include <Dy/Core/Rendering/Helper/FrameAttachmentString.h>
#include <Dy/Helper/Type/DyString.h>
#include <Dy/Management/Type/FramebufferInformation.h>
#include <Dy/Management/Internal/FramebufferManager.h>
#include <Dy/Builtin/ShaderGl/RenderFontMSDF_Deprecated.h>
#include <Dy/Helper/Type/Matrix4.h>
#include <Dy/Management/HeapResourceManager.h>
#include <Dy/Builtin/ShaderGl/RenderFontSDF_Deprecated.h>

#if !defined(FT_FREETYPE_H)
#define FT_FREETYPE_H
#endif /// FT_FREETYPE_H

//!
//! Forward declaration
//!

namespace
{

/// Freetype library pointer
FT_Library	sFreetypeLib  = nullptr;
/// Freetype face pointer used when initiating fonts.
FT_Face		  sFreetypeFace = nullptr;

MDY_NODISCARD EDySuccess DyInitializeFreetype() noexcept
{
  // Check Freetype is we`ll.
  if (FT_Init_FreeType(&sFreetypeLib) != 0) {
    MDY_UNEXPECTED_BRANCH();
    return DY_FAILURE;
  }
  return DY_SUCCESS;
}

MDY_NODISCARD EDySuccess DyLoadFontFreetype(const std::string& font_path) noexcept
{
  if (FT_New_Face(sFreetypeLib, font_path.c_str(), 0, &sFreetypeFace)) {
    MDY_UNEXPECTED_BRANCH();
    return DY_FAILURE;
  }
  return DY_SUCCESS;
}

MDY_NODISCARD EDySuccess DyReleaseFreetype() noexcept
{
  if (FT_Done_Face(sFreetypeFace) != 0) { MDY_UNEXPECTED_BRANCH(); }
  if (FT_Done_FreeType(sFreetypeLib) != 0) { MDY_UNEXPECTED_BRANCH(); }

  sFreetypeFace = nullptr;
  sFreetypeLib  = nullptr;
  return DY_SUCCESS;
}

struct DDyFreeTypeContext {
  msdfgen::Point2 position;
  msdfgen::Shape *shape;
  msdfgen::Contour *contour;
};

static msdfgen::Point2 ftPoint2(const FT_Vector &vector) {
    return msdfgen::Point2(vector.x/64., vector.y/64.);
}

static int ftMoveTo(const FT_Vector *to, void *user) {
    DDyFreeTypeContext *context = reinterpret_cast<DDyFreeTypeContext *>(user);
    context->contour = &context->shape->addContour();
    context->position = ftPoint2(*to);
    return 0;
}

static int ftLineTo(const FT_Vector *to, void *user) {
    DDyFreeTypeContext *context = reinterpret_cast<DDyFreeTypeContext *>(user);
    context->contour->addEdge(new msdfgen::LinearSegment(context->position, ftPoint2(*to)));
    context->position = ftPoint2(*to);
    return 0;
}

static int ftConicTo(const FT_Vector *control, const FT_Vector *to, void *user) {
    DDyFreeTypeContext *context = reinterpret_cast<DDyFreeTypeContext *>(user);
    context->contour->addEdge(new msdfgen::QuadraticSegment(context->position, ftPoint2(*control), ftPoint2(*to)));
    context->position = ftPoint2(*to);
    return 0;
}

static int ftCubicTo(const FT_Vector *control1, const FT_Vector *control2, const FT_Vector *to, void *user) {
    DDyFreeTypeContext *context = reinterpret_cast<DDyFreeTypeContext *>(user);
    context->contour->addEdge(new msdfgen::CubicSegment(context->position, ftPoint2(*control1), ftPoint2(*control2), ftPoint2(*to)));
    context->position = ftPoint2(*to);
    return 0;
}

} /// ::dy namesapce

//!
//! Implementation
//!

namespace dy::test
{

void TestMsdfLibrary()
{
  MDY_CALL_ASSERT_SUCCESS(DyInitializeFreetype());
  //MDY_CALL_ASSERT_SUCCESS(DyLoadFontFreetype("C:\\Windows\\Fonts\\SourceHanSerif-Medium.otf"));
  MDY_CALL_ASSERT_SUCCESS(DyLoadFontFreetype("C:\\Windows\\Fonts\\msgothic.ttc"));
  //MDY_CALL_ASSERT_SUCCESS(DyLoadFontFreetype("C:\\Windows\\Fonts\\UDDigiKyokashoN-R.ttc"));
  //MDY_CALL_ASSERT_SUCCESS(DyLoadFontFreetype("./TestResource/mincho.otf"));
  const bool isThisFontOtf = false;

  DDyString sampleText = "誕";
  FT_Error error = FT_Load_Char(sFreetypeFace, sampleText[0], FT_LOAD_NO_SCALE);
  MDY_ASSERT(error == 0, "");

  msdfgen::Shape shape;
  shape.contours.clear();
  shape.inverseYAxis = false;

  DDyFreeTypeContext context = {};
  context.shape = &shape;
  FT_Outline_Funcs ftFunctions;
  ftFunctions.move_to   = &ftMoveTo;
  ftFunctions.line_to   = &ftLineTo;
  ftFunctions.conic_to  = &ftConicTo;
  ftFunctions.cubic_to  = &ftCubicTo;
  ftFunctions.shift     = 0;
  ftFunctions.delta     = 0;
  error = FT_Outline_Decompose(&sFreetypeFace->glyph->outline, &ftFunctions, &context);
  MDY_ASSERT(error == 0, "");

  struct { double l, b, r, t; } bounds = { 1e240, 1e240, -1e240, -1e240 };
  shape.normalize();
  shape.bounds(bounds.l, bounds.b, bounds.r, bounds.t);

  double range = 1;
  const double pxRange = 4;
  const double edgeThreshold = 1;
  msdfgen::Vector2 translate;
  msdfgen::Vector2 scale = 1;

  // Auto-frame
  if (true)
  {
    double l = bounds.l, b = bounds.b, r = bounds.r, t = bounds.t;
    msdfgen::Vector2 frame(64, 64);
    //
    { frame -= 2 * pxRange; }
    if (l >= r || b >= t)             { l = 0, b = 0, r = 1, t = 1; }
    if (frame.x <= 0 || frame.y <= 0) { MDY_UNEXPECTED_BRANCH(); }
    //
    const msdfgen::Vector2 dims(r - l, t - b);
    double avgScale = 1.f;

    if (dims.x*frame.y < dims.y*frame.x)
    {
      translate.set(.5*(frame.x / frame.y*dims.y - dims.x) - l, -b);
      scale = avgScale = frame.y / dims.y;
    }
    else
    {
      translate.set(-l, .5*(frame.y / frame.x*dims.x - dims.y) - b);
      scale = avgScale = frame.x / dims.x;
    }

    translate += pxRange / scale;
  }

  if (true) { range = pxRange/msdfgen::min(scale.x, scale.y); }

  //                      max. angle
  msdfgen::edgeColoringSimple(shape, 3.0);

#ifdef false
  msdfgen::Bitmap<float> sdf(64, 64);
  msdfgen::generateSDF(sdf, shape, range, scale, translate);
  if (isThisFontOtf == true)
  {
    for (TI32 y = 0; y < 64; ++y) { for (TI32 x = 0; x < 64; ++x) { sdf(x, y) = 1 - sdf(x, y); } }
  }

  // Generate Textures
  TU32 textureId = MDY_INITIALIZE_DEFUINT;
  glGenTextures (1, &textureId);
  glBindTexture (GL_TEXTURE_2D, textureId);
  glTexImage2D  (GL_TEXTURE_2D, 0, GL_RED, 64, 64, 0, GL_RED, GL_FLOAT, &sdf(0, 0));
#endif
  //           image width, height
  msdfgen::Bitmap<msdfgen::FloatRGB> msdf(64, 64);
  //                     range, scale, translation
  msdfgen::generateMSDF(msdf, shape, range, scale, translate, edgeThreshold);

  // Generate Textures
  TU32 textureId = MDY_INITIALIZE_DEFUINT;
  glGenTextures(1, &textureId);
  glBindTexture(GL_TEXTURE_2D, textureId);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 64, 64, 0, GL_RGB, GL_FLOAT, &msdf(0, 0));
  // Set Texture Options
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  // Store character for later use
  glBindTexture(GL_TEXTURE_2D, 0);
  MDY_CALL_ASSERT_SUCCESS(DyReleaseFreetype());

  // Create framebuffer and attachment. (sample)
  PDyGlFrameBufferInformation       framebufferInfo = {};
  PDyGlAttachmentInformation        attachmentInfo  = {};
  PDyGlAttachmentBinderInformation  binderInfo      = {};

  framebufferInfo.mFrameBufferName = sFrameBuffer_Test;
  framebufferInfo.mFrameBufferSize = DDyVectorInt2{1280, 720};
  framebufferInfo.mIsUsingDefaultDepthBuffer = true;

  // Rendered texture buffer
  attachmentInfo.mAttachmentName = sAttachment_Test_Output;
  attachmentInfo.mAttachmentSize = DDyVectorInt2{1280, 720};
  attachmentInfo.mParameterList  = {
    PDyGlTexParameterInformation\
    {EDyGlParameterName::TextureMinFilter, EDyGlParameterValue::Nearest},
    {EDyGlParameterName::TextureMagFilter, EDyGlParameterValue::Nearest},
    {EDyGlParameterName::TextureWrappingS, EDyGlParameterValue::ClampToBorder},
    {EDyGlParameterName::TextureWrappingT, EDyGlParameterValue::ClampToBorder},
  };
  attachmentInfo.mBorderColor = DDyColor{0, 0, 0, 0};

  binderInfo.mAttachmentName  = sAttachment_Output;
  binderInfo.mAttachmentType  = EDyGlAttachmentType::Color0;
  framebufferInfo.mAttachmentList.push_back(binderInfo);

  // Push attachment buffer
  auto& framebufferManager  = MDyFramebuffer::GetInstance();
  MDY_CALL_ASSERT_SUCCESS(framebufferManager.SetAttachmentInformation(attachmentInfo));
  // Create framebuffer.
  MDY_CALL_ASSERT_SUCCESS(framebufferManager.InitializeNewFrameBuffer(framebufferInfo));
  PDyGlFrameBufferInformation* mDyBtFbTest = framebufferManager.GetFrameBufferPointer(MSVSTR(sFrameBuffer_Test));

  // Shader create
  // Create projection and uniform informations
  builtin::FDyBuiltinShaderGLRenderFontMSDF_Deprecated();
  //builtin::FDyBuiltinShaderGLRenderFontSDF_Deprecated();
  DDyMatrix4x4 uUiProjMatrix = glm::ortho<float>(-640, 640, -360, 360, 0.2f, 10.0f);
  CDyShaderResource* sSampleShaderPtr = MDyHeapResource::GetInstance().GetShaderResource(MSVSTR(builtin::FDyBuiltinShaderGLRenderFontMSDF_Deprecated::sName));
  //CDyShaderResource* sSampleShaderPtr = MDyHeapResource::GetInstance().GetShaderResource(MSVSTR(builtin::FDyBuiltinShaderGLRenderFontSDF_Deprecated::sName));

  //!
  //! Make vbo sample
  //!
  GLuint mTextSampleVao = MDY_INITIALIZE_DEFUINT;
  GLuint mTextSampleVbo = MDY_INITIALIZE_DEFUINT;
  constexpr auto stride = sizeof(DDyVector2) * 2;

  static auto MakeSampleVbo = [&]()
  {
    glGenVertexArrays(1, &mTextSampleVao);
    glGenBuffers(1, &mTextSampleVbo);

    glBindVertexArray(mTextSampleVao);
    glBindBuffer(GL_ARRAY_BUFFER, mTextSampleVbo);

    constexpr float size = 256;
    std::array<DDyVector2, 8> value =
    {
      DDyVector2{ size,  size}, DDyVector2{1, 1},
      DDyVector2{ size, -size}, DDyVector2{1, 0},
      DDyVector2{-size, -size}, DDyVector2{0, 0},
      DDyVector2{-size,  size}, DDyVector2{0, 1}
    };
    glBufferData(GL_ARRAY_BUFFER, stride * 4, value.data(), GL_DYNAMIC_DRAW);

    glBindVertexBuffer(0, mTextSampleVbo, 0, stride);
    glEnableVertexAttribArray(0);
    glVertexAttribFormat(0, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexAttribBinding(0, 0);
    glEnableVertexAttribArray(1);
    glVertexAttribFormat(1, 2, GL_FLOAT, GL_FALSE, 8);
    glVertexAttribBinding(1, 0);

    glBindVertexArray(0);
  };
  MakeSampleVbo();

  //!
  //! Render '뷁' to center of screen.
  //!
  glBindFramebuffer (GL_FRAMEBUFFER, mDyBtFbTest->GetFramebufferId());
  glDepthFunc       (GL_ALWAYS);
  sSampleShaderPtr->UseShader();
  glBindVertexArray (mTextSampleVao);

  //
  const auto uUiProjMatrixId = glGetUniformLocation(sSampleShaderPtr->GetShaderProgramId(), "uUiProjMatrix");
  glUniformMatrix4fv(uUiProjMatrixId, 1, GL_FALSE, &uUiProjMatrix[0].X);
  const auto pxRangeId       = glGetUniformLocation(sSampleShaderPtr->GetShaderProgramId(), "pxRange");
  glUniform1f(pxRangeId, pxRange);
  const auto bgColorId       = glGetUniformLocation(sSampleShaderPtr->GetShaderProgramId(), "bgColor");
  const DDyColor bgColor     = DDyColor{1, 1, 1, 1};
  glUniform4fv(bgColorId, 1, &bgColor.R);
  const auto fgColorId       = glGetUniformLocation(sSampleShaderPtr->GetShaderProgramId(), "fgColor");
  const DDyColor fgColor     = DDyColor{0, 0, 0, 1};
  glUniform4fv(fgColorId, 1, &fgColor.R);

  // Render texture glyph
  glActiveTexture   (GL_TEXTURE0);
  glBindTexture     (GL_TEXTURE_2D, textureId);
  glDrawArrays      (GL_TRIANGLE_FAN, 0, 4);

  glBindVertexArray (0);
  sSampleShaderPtr->UnuseShader();
  glDepthFunc       (GL_LEQUAL);
  glBindFramebuffer (GL_FRAMEBUFFER, 0);
}

} /// ::dy::test namespace