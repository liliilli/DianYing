#ifndef GUARD_DY_MANAGEMENT_IO_IODATAMANANGER_20181113_H
#define GUARD_DY_MANAGEMENT_IO_IODATAMANANGER_20181113_H
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

#include <Dy/Helper/Wrapper/DDyMutexUniqueHashMap.h>
#include <Dy/Core/Resource/Information/FDyShaderInformation.h>
#include <Dy/Core/Resource/Information/FDyTextureInformation.h>
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

template <EDyResourceType TType> struct TDyRscInfo;
template <>
struct TDyRscInfo<EDyResourceType::Texture> final { using type = FDyTextureInformation; };
template <>
struct TDyRscInfo<EDyResourceType::GLShader> final { using type = FDyShaderInformation; };

///
/// @class MDyIOData
/// @brief IO Data Manager (newly implemented)
///
class MDyIOData final : public IDySingleton<MDyIOData>
{
  MDY_SINGLETON_PROPERTIES(MDyIOData);
  MDY_SINGLETON_DERIVED(MDyIOData);
public:
  template <EDyResourceType TType>
  MDY_NODISCARD typename TDyRscInfo<TType>::type* GetPtrInformation(_MIN_ const std::string& specifier) noexcept
  {
    if constexpr (TType == EDyResourceType::GLShader)
    {
      return this->__mShaderContainer.TryGetInstancePtr(specifier);
    }
    else if constexpr (TType == EDyResourceType::Texture)
    {
      return this->__mTextureContainer.TryGetInstancePtr(specifier);
    }
    else
    {
      MDY_NOT_IMPLEMENTED_ASSERT();
      return nullptr;
    }
  }

  template <EDyResourceType TType>
  MDY_NODISCARD const typename TDyRscInfo<TType>::type* GetPtrInformation(_MIN_ const std::string& specifier) const noexcept
  {
    if constexpr (TType == EDyResourceType::GLShader)
    {
      return this->__mShaderContainer.TryGetInstancePtr(specifier);
    }
    else if constexpr (TType == EDyResourceType::Texture)
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

  __THashMap<FDyShaderInformation>  __mShaderContainer = {};
  __THashMap<FDyTextureInformation> __mTextureContainer = {};

  friend class TDyIO;
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_IO_IODATAMANANGER_20181113_H