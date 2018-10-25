#ifndef GUARD_DY_MANAGEMENT_TYPE_UBOINFORMATION_H
#define GUARD_DY_MANAGEMENT_TYPE_UBOINFORMATION_H
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

#include <Dy/Helper/GlobalType.h>
#include <Dy/Helper/Macroes.h>

namespace dy
{

///
/// @enum   EDyBufferDrawType
/// @brief  Buffer draw type for binding to gpu for use.
///
enum class EDyBufferDrawType
{
  StaticDraw,
  DynamicDraw,
};

///
/// @brief  Get buffer draw type value.
/// @param  type Type value.
/// @return OpenGL Type value
///
inline MDY_NODISCARD GLenum DyGetBufferDrawTypeValue(_MIN_ const EDyBufferDrawType type) noexcept
{
  GLenum retValue = GL_NONE;
  switch (type)
  {
  case EDyBufferDrawType::StaticDraw:   retValue = GL_STATIC_DRAW;  break;
  case EDyBufferDrawType::DynamicDraw:  retValue = GL_DYNAMIC_DRAW; break;
  }
  return retValue;
}

///
/// @struct PDyUboConstructionDescriptor
/// @brief  Uniform buffer object consturction descriptor.
///
struct PDyUboConstructionDescriptor final
{
  // This specifier name must not be duplicated with others.
  std::string       mUboSpecifierName = MDY_INITILAIZE_EMPTYSTR;
  // Buffer drawing type.
  EDyBufferDrawType mBufferDrawType   = EDyBufferDrawType::StaticDraw;
  // Binding index must be not duplicated with valid ubo instance binding point.
  TU32              mBindingIndex     = MDY_INITIALIZE_DEFUINT;
  // Buffer must be valid until instance is made.
  void*             mBufferStartPtr   = MDY_INITIALIZE_NULL;
  // UBO buffer size for each element.
  TU32              mUboElementSize   = MDY_INITIALIZE_DEFUINT;
  // UBO array size count.
  TU32              mUboArraySize     = 1;
};

///
/// @struct DDyUboInstanceInformation
/// @brief
///
struct DDyUboInstanceInformation final
{
public:
  ///
  /// @param desc Descriptor instance.
  ///
  DDyUboInstanceInformation(_MIN_ const PDyUboConstructionDescriptor& desc) :
      mUboSpecifierName(desc.mUboSpecifierName)
    , mBufferDrawType(desc.mBufferDrawType)
    , mBindingIndex(desc.mBindingIndex)
    , mUboElementSize(desc.mUboElementSize)
    , mUboArraySize(desc.mUboArraySize)
    , mContainerSize(desc.mUboArraySize * desc.mUboElementSize)
  {};

  // This specifier name must not be duplicated with others.
  std::string       mUboSpecifierName = MDY_INITILAIZE_EMPTYSTR;
  // Buffer drawing type.
  EDyBufferDrawType mBufferDrawType   = EDyBufferDrawType::StaticDraw;
  // Binding index must be not duplicated with valid ubo instance binding point.
  TU32              mBindingIndex     = MDY_INITIALIZE_DEFUINT;
  // UBO buffer size for each element.
  TU32              mUboElementSize   = MDY_INITIALIZE_DEFUINT;
  // UBO array size count.
  TU32              mUboArraySize     = 1;

  ///
  /// @brief  Total container size of ubo.
  /// @return
  ///
  FORCEINLINE MDY_NODISCARD TU32 GetContainerSize() const noexcept
  {
    return this->mContainerSize;
  }

  ///
  /// @brief  Buffer index of this ubo. (Not binding index!)
  /// @return
  ///
  FORCEINLINE MDY_NODISCARD TU32 GetBufferInternalIndex() const noexcept
  {
    return this->mBufferIndex;
  }

private:
  // Ubo total container size. requested for verifying buffer oob.
  TU32              mContainerSize    = MDY_INITIALIZE_DEFUINT;
  // Ubo given buffer index.
  TU32              mBufferIndex      = MDY_INITIALIZE_DEFUINT;

  friend class MDyUniformBufferObject;
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_TYPE_UBOINFORMATION_H