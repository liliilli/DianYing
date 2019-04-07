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

#include <Dy/Helper/GlobalType.h>
#include <Dy/Helper/System/Macroes.h>

namespace dy
{

/// @enum   EDyBufferDrawType
/// @brief  Buffer draw type for binding to gpu for use.
enum class EDyBufferDrawType
{
  StaticDraw,
  DynamicDraw,
};

/// @brief  Get buffer draw type value.
/// @param  type Type value.
/// @return OpenGL Type value
TGlEnum DyGetBufferDrawTypeValue(EDyBufferDrawType type) noexcept;

/// @struct PDyUboConstructionDescriptor
/// @brief  Uniform buffer object consturction descriptor.
struct PDyUboConstructionDescriptor final
{
  // This specifier name must not be duplicated with others.
  std::string       mUboSpecifierName;
  // Buffer drawing type.
  EDyBufferDrawType mBufferDrawType   = EDyBufferDrawType::StaticDraw;
  // Binding index must be not duplicated with valid ubo instance binding point.
  TU32              mBindingIndex     = 0;
  // Buffer must be valid until instance is made.
  void*             mBufferStartPtr   = nullptr;
  // UBO buffer size for each element.
  TU32              mUboElementSize   = 0;
  // UBO array size count.
  TU32              mUboArraySize     = 1;
};

/// @struct DDyUboInstanceInformation
/// @brief
struct DDyUboInstanceInformation final
{
public:
  /// @param desc Descriptor instance.
  DDyUboInstanceInformation(const PDyUboConstructionDescriptor& desc) :
      mUboSpecifierName(desc.mUboSpecifierName)
    , mBufferDrawType(desc.mBufferDrawType)
    , mBindingIndex(desc.mBindingIndex)
    , mUboElementSize(desc.mUboElementSize)
    , mUboArraySize(desc.mUboArraySize)
    , mContainerSize(desc.mUboArraySize * desc.mUboElementSize)
  {};

  // This specifier name must not be duplicated with others.
  std::string       mUboSpecifierName;
  // Buffer drawing type.
  EDyBufferDrawType mBufferDrawType   = EDyBufferDrawType::StaticDraw;
  // Binding index must be not duplicated with valid ubo instance binding point.
  TU32              mBindingIndex     = 0;
  // UBO buffer size for each element.
  TU32              mUboElementSize   = 0;
  // UBO array size count.
  TU32              mUboArraySize     = 1;

  /// @brief  Total container size of ubo.
  /// @return
  TU32 GetContainerSize() const noexcept
  {
    return this->mContainerSize;
  }

  /// @brief  Buffer index of this ubo. (Not binding index!)
  /// @return
  TU32 GetBufferInternalIndex() const noexcept
  {
    return this->mBufferIndex;
  }

private:
  // Ubo total container size. requested for verifying buffer oob.
  TU32 mContainerSize = MDY_INITIALIZE_DEFUINT;
  // Ubo given buffer index.
  TU32 mBufferIndex   = MDY_INITIALIZE_DEFUINT;

  friend class MUniformBufferObject;
};

} /// ::dy namespace
