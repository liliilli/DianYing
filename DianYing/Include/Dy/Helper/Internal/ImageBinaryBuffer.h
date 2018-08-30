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
/// @brief
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
/// @brief RAII
///
class DDyImageBinaryDataBuffer final
{
public:
  DDyImageBinaryDataBuffer(const std::string& imagePath);
  ~DDyImageBinaryDataBuffer();

  DDyImageBinaryDataBuffer(const DDyImageBinaryDataBuffer&)       = delete;
  DDyImageBinaryDataBuffer& operator=(DDyImageBinaryDataBuffer&)  = delete;
  DDyImageBinaryDataBuffer(DDyImageBinaryDataBuffer&&) noexcept             = default;
  DDyImageBinaryDataBuffer& operator=(DDyImageBinaryDataBuffer&&) noexcept  = default;

  ///
  /// @brief
  ///
  [[nodiscard]] bool IsBufferCreatedProperly() const noexcept
  {
    return mIsBufferCreatedProperly;
  }

  ///
  /// @brief
  ///
  [[nodiscard]] int32_t GetImageWidth() const noexcept
  {
    return mWidth;
  }

  ///
  /// @brief
  ///
  [[nodiscard]] int32_t GetImageHeight() const noexcept
  {
    return mHeight;
  }

  ///
  /// @brief
  ///
  [[nodiscard]] EDyImageColorFormatStyle GetImageFormat() const noexcept
  {
    return mImageFormat;
  }

  ///
  /// @brief
  ///
  const unsigned char* GetBufferStartPoint() const noexcept
  {
    return mBufferStartPoint;
  }

private:
  std::string mImagePath      = "";
  int32_t     mImageChannel   = MDY_NOT_INITIALIZED_M1;
  int32_t     mWidth          = MDY_NOT_INITIALIZED_M1;
  int32_t     mHeight         = MDY_NOT_INITIALIZED_M1;
  int32_t     mDesiredChannel = MDY_NOT_INITIALIZED_M1;
  EDyImageColorFormatStyle mImageFormat = EDyImageColorFormatStyle::NoneError;
  unsigned char* mBufferStartPoint = nullptr;

  bool mIsBufferCreatedProperly = false;
};

} /// ::dy namespace

#endif /// GUARD_DY_IMAGE_BINARY_BUFFER_H