#ifndef GUARD_DY_CORE_RESOURCE_TYPE_TEMPLATERESCINFOTYPE_H
#define GUARD_DY_CORE_RESOURCE_TYPE_TEMPLATERESCINFOTYPE_H
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

#include <Dy/Meta/Type/EDyResourceType.h>

//!
//! Forward declaration
//!

namespace dy
{
class FDyTextureInformation;
class FDyShaderInformation;
class FDyModelInformation;
class FDyMaterialInformation;

class FDyModelResource;
class FDyTextureResource;
class FDyShaderResource;
class FDyMaterialResource;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

template <EDyResourceType TType> struct __TDyRscInfo;
template <> struct __TDyRscInfo<EDyResourceType::Texture> final { using type = FDyTextureInformation; };
template <> struct __TDyRscInfo<EDyResourceType::GLShader> final { using type = FDyShaderInformation; };
template <> struct __TDyRscInfo<EDyResourceType::Model> final { using type = FDyModelInformation; };
template <> struct __TDyRscInfo<EDyResourceType::Material> final { using type = FDyMaterialInformation; };
template <EDyResourceType TType> using __TDyRscInfo_T = typename __TDyRscInfo<TType>::type;

template <EDyResourceType TType> struct __TResourceType;
template <> struct __TResourceType<EDyResourceType::Texture> final  { using type = FDyTextureResource; };
template <> struct __TResourceType<EDyResourceType::GLShader> final { using type = FDyShaderResource; };
template <> struct __TResourceType<EDyResourceType::Model> final    { using type = FDyModelResource; };
template <> struct __TResourceType<EDyResourceType::Material> final { using type = FDyMaterialResource; };
template <EDyResourceType TType> using __TResourceType_T = typename __TResourceType<TType>::type;

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_TYPE_TEMPLATERESCINFOTYPE_H