#ifndef GUARD_DY_MANAGEMENT_IO_MDYIORESOURCE_20181113_H
#define GUARD_DY_MANAGEMENT_IO_MDYIORESOURCE_20181113_H
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

#include <Dy/Core/Resource/Resource/FDyShaderResource.h>
#include <Dy/Core/Resource/Resource/FDyTextureResource.h>
#include <Dy/Core/Resource/Resource/FDyModelResource.h>
#include <Dy/Helper/Wrapper/DDyMutexUniqueHashMap.h>
#include <Dy/Management/Interface/ISingletonCrtp.h>
#include <Dy/Meta/Type/EDyResourceType.h>

//!
//! Forward declaration
//!

namespace dy
{
enum class EDyResourceType;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

template <EDyResourceType TType> struct TDyRscResc;
template <>
struct TDyRscResc<EDyResourceType::Texture> final { using type = FDyTextureResource; };
template <>
struct TDyRscResc<EDyResourceType::GLShader> final { using type = FDyShaderResource; };

///
/// @class MDyIOResource
/// @brief IO Resource part manager (newly implemented)
///
class MDyIOResource final : public IDySingleton<MDyIOResource>
{
  MDY_SINGLETON_PROPERTIES(MDyIOResource);
  MDY_SINGLETON_DERIVED(MDyIOResource);
public:
  template <EDyResourceType TType>
  MDY_NODISCARD typename TDyRscResc<TType>::type* GetPtrInformation(_MIN_ const std::string& specifier)
  {
    if constexpr (std::is_same_v<TType, EDyResourceType::GLShader> == true)
    {
      return this->__mShaderContainer.TryGetInstancePtr(specifier);
    }
    else if constexpr (std::is_same_v<TType, EDyResourceType::Texture> == true)
    {
      return this->__mTextureContainer.TryGetInstancePtr(specifier);
    }
    else
    {
      MDY_NOT_IMPLEMENTED_ASSERT();
      return nullptr;
    }
  }

private:
  /// @brief Insert result from TDyIO (I/O GC-IN Phase)
  void InsertResult(_MIN_ EDyResourceType type, _MIN_ void* ptrrawInstance);

  template <typename TType>
  using __THashMap = DDyMutexUniqueHashMap<std::string, TType>;

  __THashMap<FDyShaderResource>   __mShaderContainer   = {};
  __THashMap<FDyTextureResource>  __mTextureContainer  = {};
  __THashMap<FDyModelResource>    __mModelContainer    = {};

  friend class TDyIO;
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_IO_IODATAMANANGER_20181113_H