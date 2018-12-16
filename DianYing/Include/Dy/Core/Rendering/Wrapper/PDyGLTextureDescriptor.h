#ifndef GUARD_DY_CORE_RENDERING_WRAPPER_PDYGLTEXTUREDESCRIPTOR_H
#define GUARD_DY_CORE_RENDERING_WRAPPER_PDYGLTEXTUREDESCRIPTOR_H
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

#include <Dy/Helper/Type/VectorInt2.h>
#include <Dy/Core/Resource/Internal/TextureEnums.h>
#include <Dy/Meta/Information/MetaInfoTexture.h>

namespace dy
{

///
/// @struct PDyGLTextureDescriptor
/// @brief Descriptor instance for creating texture context of opengl.
///
struct PDyGLTextureDescriptor final
{
  EDyTextureStyleType mType               = EDyTextureStyleType::NoneError;
  bool                mIsUsingDefaultMipmap       = false;
  bool                mIsUsingCustomizedParameter = false;
  TI32                mImageFormat        = MDY_INITIALIZE_DEFINT;
  DDyVectorInt2       mTextureSize        = {};
  DDyColorRGBA        mBorderColor        = {};

  const std::vector<TU08>*     mPtrBuffer        = MDY_INITIALIZE_NULL;
  const TTextureParameterList* mPtrParameterList = MDY_INITIALIZE_NULL;

  TU32                mOutputTextureId    = MDY_INITIALIZE_DEFUINT;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RENDERING_WRAPPER_PDYGLTEXTUREDESCRIPTOR_H