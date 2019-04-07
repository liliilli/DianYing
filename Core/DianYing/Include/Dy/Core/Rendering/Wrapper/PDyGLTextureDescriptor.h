#ifndef GUARD_DY_CORE_RENDERING_WRAPPER_PDYGLTEXTUREDESCRIPTOR_H
#define GUARD_DY_CORE_RENDERING_WRAPPER_PDYGLTEXTUREDESCRIPTOR_H
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

#include <Dy/Helper/Type/DVector2.h>
#include <Dy/Core/Resource/Internal/ETextureEnums.h>
#include <Dy/Meta/Information/MetaInfoTexture.h>

namespace dy
{

/// @struct PDyGLTextureBase
/// @brief Base descriptor type of `PDyGLTextureDescriptor` and `PDyGLTextureCubemapDescriptor`.
struct PDyGLTextureBase
{
  PDyGLTextureBase() = default;
  virtual ~PDyGLTextureBase() = 0;

  ETextureStyleType mType = ETextureStyleType::NoneError;
  bool mIsUsingDefaultMipmap = false;
  bool mIsUsingCustomizedParameter = false;
  TI32 mImageFormat = -1;
  TGlEnum mImagePixelType = 0;
  DColorRGBA mBorderColor = {};

  const TTextureParameterList* mPtrParameterList = nullptr;
  TU32 mOutputTextureId = 0;

  using TPtrBuffer = const std::vector<TU8>*;
};

inline PDyGLTextureBase::~PDyGLTextureBase() = default;

/// @struct PDyGLTextureDescriptor
/// @brief Descriptor instance for creating texture context of opengl.
struct PDyGLTextureDescriptor final : public PDyGLTextureBase
{
  TPtrBuffer mPtrBuffer = MDY_INITIALIZE_NULL;
  DIVec2 mTextureSize = {};
};

/// @struct PDyGLTextureCubemapDescriptor
/// @brief Descriptor instance for creating `cubemap` texture context of opengl.
struct PDyGLTextureCubemapDescriptor final : public PDyGLTextureBase
{
  TPtrBuffer mTopBuffer    = nullptr;
  TPtrBuffer mBottomBuffer = nullptr;
  TPtrBuffer mLeftBuffer   = nullptr;
  TPtrBuffer mRightBuffer  = nullptr;
  TPtrBuffer mFrontBuffer  = nullptr;
  TPtrBuffer mBackBuffer   = nullptr;

  DIVec2 mTopSize    = {};
  DIVec2 mBottomSize = {};
  DIVec2 mLeftSize   = {};
  DIVec2 mRightSize  = {};
  DIVec2 mFrontSize  = {};
  DIVec2 mBackSize   = {};
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RENDERING_WRAPPER_PDYGLTEXTUREDESCRIPTOR_H