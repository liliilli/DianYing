#ifndef GUARD_DY_IMAGE_BINARY_BUFFER_H
#define GUARD_DY_IMAGE_BINARY_BUFFER_H
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

#include <string>
#include <nlohmann/json_fwd.hpp>

namespace dy
{

/// @enum EDyImageColorFormatStyle
/// @brief Image color format which is a type of each pixel data.
enum class EDyImageColorFormatStyle
{
  NoneError,  // Do not use this.
  R,          // Red-Green 8bit unsigned.
  RG,         // Red-Green 8bit unsigned.
  RGB,        // Red-Green-Blue 8bit unsigned.
  RGBA        // Red-Green-Blue and alpha 8bit unsigned.
};

/// @brief Serialization function.
void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const EDyImageColorFormatStyle& p);
/// @brief Deerialization function.
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ EDyImageColorFormatStyle& p);

/// @brief Get internal GLenum for texture formatting from style.
/// If errored, just return null value.
MDY_NODISCARD std::optional<GLenum> GlGetImageFormatFrom(_MIN_ EDyImageColorFormatStyle style) noexcept;

/// @enum EDyGlImagePixelReadType
/// @brief Image reading pixel's type.
enum class EDyGlImagePixelReadType
{
  NoneError,
  UnsignedByte, // GL_UNSIGNED_BYTE
  Float,        // GL_FLOAT
};

/// @brief Serialization function.
void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const EDyGlImagePixelReadType& p);
/// @brief Deerialization function.
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ EDyGlImagePixelReadType& p);

/// @brief Get internal image format type.
MDY_NODISCARD GLenum GlGetImagePixelTypeFrom(_MIN_ EDyGlImagePixelReadType iType) noexcept;

/// @class DImageBinaryBuffer
/// @brief Image binary buffer that manages binary buffer chunk, 
/// automatically released when it's be out of scope.
class DImageBinaryBuffer final
{
public:
  DImageBinaryBuffer(const std::string& imagePath);
  ~DImageBinaryBuffer();

  DImageBinaryBuffer(const DImageBinaryBuffer&)                 = delete;
  DImageBinaryBuffer& operator=(DImageBinaryBuffer&)            = delete;
  DImageBinaryBuffer(DImageBinaryBuffer&&) noexcept             = default;
  DImageBinaryBuffer& operator=(DImageBinaryBuffer&&) noexcept  = default;

  /// @brief Check if buffer chunk is created properly when construction time.
  MDY_NODISCARD bool IsBufferCreatedProperly() const noexcept
  {
    return this->mIsBufferCreatedProperly;
  }

  /// @brief Get image width.
  MDY_NODISCARD TI32 GetImageWidth() const noexcept
  {
    return this->mWidth;
  }

  /// @brief Get image height.
  MDY_NODISCARD TI32 GetImageHeight() const noexcept
  {
    return this->mHeight;
  }

  /// @brief Get image format value.
  MDY_NODISCARD EDyImageColorFormatStyle GetImageFormat() const noexcept
  {
    return this->mImageFormat;
  }

  /// @brief Get the start point of binary buffer chunk.
  MDY_NODISCARD const unsigned char* GetBufferStartPoint() const noexcept
  {
    return this->mBufferStartPoint;
  }

private:
  int32_t mImageChannel   = MDY_INITIALIZE_DEFINT;
  int32_t mWidth          = MDY_INITIALIZE_DEFINT;
  int32_t mHeight         = MDY_INITIALIZE_DEFINT;
  int32_t mDesiredChannel = MDY_INITIALIZE_DEFINT;
  EDyImageColorFormatStyle mImageFormat = EDyImageColorFormatStyle::NoneError;
  unsigned char* mBufferStartPoint      = nullptr;
  bool mIsBufferCreatedProperly         = false;
};

} /// ::dy namespace

#endif /// GUARD_DY_IMAGE_BINARY_BUFFER_H