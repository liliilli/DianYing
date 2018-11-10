#ifndef GUARD_DY_IMAGE_BINARY_BUFFER_H
#define GUARD_DY_IMAGE_BINARY_BUFFER_H
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

#include <string>

namespace dy
{

///
/// @enum EDyImageColorFormatStyle
/// @brief Image color format which is a type of each pixel data.
///
enum class EDyImageColorFormatStyle
{
  NoneError,
  R,
  RG,
  RGB,
  RGBA
};

///
/// @class DDyImageBinaryDataBuffer
/// @brief Image binary buffer that manages binary buffer chunk, automatically released when it's be out of scope.
///
class DDyImageBinaryDataBuffer final
{
public:
  DDyImageBinaryDataBuffer(const std::string& imagePath);
  ~DDyImageBinaryDataBuffer();

  DDyImageBinaryDataBuffer(const DDyImageBinaryDataBuffer&)                 = delete;
  DDyImageBinaryDataBuffer& operator=(DDyImageBinaryDataBuffer&)            = delete;
  DDyImageBinaryDataBuffer(DDyImageBinaryDataBuffer&&) noexcept             = default;
  DDyImageBinaryDataBuffer& operator=(DDyImageBinaryDataBuffer&&) noexcept  = default;

  ///
  /// @brief Check if buffer chunk is created properly when construction time.
  ///
  MDY_NODISCARD bool IsBufferCreatedProperly() const noexcept
  {
    return this->mIsBufferCreatedProperly;
  }

  ///
  /// @brief Get image width.
  ///
  MDY_NODISCARD TI32 GetImageWidth() const noexcept
  {
    return this->mWidth;
  }

  ///
  /// @brief Get image height.
  ///
  MDY_NODISCARD TI32 GetImageHeight() const noexcept
  {
    return this->mHeight;
  }

  ///
  /// @brief Get image format value.
  ///
  MDY_NODISCARD EDyImageColorFormatStyle GetImageFormat() const noexcept
  {
    return this->mImageFormat;
  }

  ///
  /// @brief Get the start point of binary buffer chunk.
  ///
  MDY_NODISCARD const unsigned char* GetBufferStartPoint() const noexcept
  {
    return this->mBufferStartPoint;
  }

private:
  int32_t     mImageChannel   = MDY_INITIALIZE_DEFINT;
  int32_t     mWidth          = MDY_INITIALIZE_DEFINT;
  int32_t     mHeight         = MDY_INITIALIZE_DEFINT;
  int32_t     mDesiredChannel = MDY_INITIALIZE_DEFINT;
  EDyImageColorFormatStyle mImageFormat = EDyImageColorFormatStyle::NoneError;
  unsigned char* mBufferStartPoint      = nullptr;
  bool mIsBufferCreatedProperly         = false;
};

} /// ::dy namespace

#endif /// GUARD_DY_IMAGE_BINARY_BUFFER_H