#ifndef GUARD_DY_CORE_RESOURCE_TYPE_TEMPLATERESCINFOTYPE_H
#define GUARD_DY_CORE_RESOURCE_TYPE_TEMPLATERESCINFOTYPE_H
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

#include <Dy/Meta/Type/EResourceType.h>

//!
//! Forward declaration
//!

namespace dy
{
class FDyMeshInformation;
class FDyTextureInformation;
class FDyShaderInformation;
class FDyModelInformation;
class FDyModelSkeletonInformation;
class FDyMaterialInformation;
class FDyModelAnimScrapInformation;
class FDyAttachmentInformation;
class FDyFrameBufferInformation;
class FDySoundInformation;

class FDyMeshResource;
class FDyModelResource;
class FDyTextureResource;
class FDyShaderResource;
class FDyMaterialResource;
class FDyAttachmentResource;
class FDyFrameBufferResource;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

template <EResourceType TType> struct __TDyRscInfo;
template <> struct __TDyRscInfo<EResourceType::Texture> final { using type = FDyTextureInformation; };
template <> struct __TDyRscInfo<EResourceType::GLShader> final { using type = FDyShaderInformation; };
template <> struct __TDyRscInfo<EResourceType::Mesh> final { using type = FDyMeshInformation; };
template <> struct __TDyRscInfo<EResourceType::Model> final { using type = FDyModelInformation; };
template <> struct __TDyRscInfo<EResourceType::Skeleton> final { using type = FDyModelSkeletonInformation; };
template <> struct __TDyRscInfo<EResourceType::Sound> final { using type = FDySoundInformation; };
template <> struct __TDyRscInfo<EResourceType::AnimationScrap> final { using type = FDyModelAnimScrapInformation; };
template <> struct __TDyRscInfo<EResourceType::Material> final { using type = FDyMaterialInformation; };
template <> struct __TDyRscInfo<EResourceType::GLAttachment> final { using type = FDyAttachmentInformation; };
template <> struct __TDyRscInfo<EResourceType::GLFrameBuffer> final { using type = FDyFrameBufferInformation; };
template <EResourceType TType> using __TDyRscInfo_T = typename __TDyRscInfo<TType>::type;

template <EResourceType TType> struct __TResourceType;
template <> struct __TResourceType<EResourceType::Texture> final  { using type = FDyTextureResource; };
template <> struct __TResourceType<EResourceType::GLShader> final { using type = FDyShaderResource; };
template <> struct __TResourceType<EResourceType::Mesh> final     { using type = FDyMeshResource; };
template <> struct __TResourceType<EResourceType::Model> final    { using type = FDyModelResource; };
template <> struct __TResourceType<EResourceType::Material> final { using type = FDyMaterialResource; };
template <> struct __TResourceType<EResourceType::GLAttachment> final { using type = FDyAttachmentResource; };
template <> struct __TResourceType<EResourceType::GLFrameBuffer> final { using type = FDyFrameBufferResource; };
template <EResourceType TType> using __TResourceType_T = typename __TResourceType<TType>::type;

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_TYPE_TEMPLATERESCINFOTYPE_H