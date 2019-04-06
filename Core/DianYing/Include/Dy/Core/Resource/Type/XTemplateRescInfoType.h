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
class FInformationMesh;
class FInformationTexture;
class FInformationShader;
class FInformationModel;
class FInformationModelSkeleton;
class FInformationMaterial;
class FInformationModelAnimScrap;
class FInformationAttachment;
class FInformationFrameBuffer;
class FInformationSound;

class FResourceMesh;
class FResourceModel;
class AResourceTextureBase;
class FResourceShader;
class FResourceMaterial;
class AResourceAttachmentBase;
class AResourceFrameBufferBase;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

template <EResourceType TType> struct __TDyRscInfo;
template <> struct __TDyRscInfo<EResourceType::Texture> final { using type = FInformationTexture; };
template <> struct __TDyRscInfo<EResourceType::GLShader> final { using type = FInformationShader; };
template <> struct __TDyRscInfo<EResourceType::Mesh> final { using type = FInformationMesh; };
template <> struct __TDyRscInfo<EResourceType::Model> final { using type = FInformationModel; };
template <> struct __TDyRscInfo<EResourceType::Skeleton> final { using type = FInformationModelSkeleton; };
template <> struct __TDyRscInfo<EResourceType::Sound> final { using type = FInformationSound; };
template <> struct __TDyRscInfo<EResourceType::AnimationScrap> final { using type = FInformationModelAnimScrap; };
template <> struct __TDyRscInfo<EResourceType::Material> final { using type = FInformationMaterial; };
template <> struct __TDyRscInfo<EResourceType::GLAttachment> final { using type = FInformationAttachment; };
template <> struct __TDyRscInfo<EResourceType::GLFrameBuffer> final { using type = FInformationFrameBuffer; };
template <EResourceType TType> using __TDyRscInfo_T = typename __TDyRscInfo<TType>::type;

template <EResourceType TType> struct __TResourceType;
template <> struct __TResourceType<EResourceType::Texture> final  { using type = AResourceTextureBase; };
template <> struct __TResourceType<EResourceType::GLShader> final { using type = FResourceShader; };
template <> struct __TResourceType<EResourceType::Mesh> final     { using type = FResourceMesh; };
template <> struct __TResourceType<EResourceType::Model> final    { using type = FResourceModel; };
template <> struct __TResourceType<EResourceType::Material> final { using type = FResourceMaterial; };
template <> struct __TResourceType<EResourceType::GLAttachment> final { using type = AResourceAttachmentBase; };
template <> struct __TResourceType<EResourceType::GLFrameBuffer> final { using type = AResourceFrameBufferBase; };
template <EResourceType TType> using __TResourceType_T = typename __TResourceType<TType>::type;

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_TYPE_TEMPLATERESCINFOTYPE_H