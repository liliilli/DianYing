#include <precompiled.h>
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

/// Header file
#include <Dy/Helper/Library/HelperZlib.h>
/// Libraries
#include <zlib/zlib.h>

//!
//! Forward declaration & local translation unit function
//!

namespace
{

///
/// @struct DHeaderStructure
/// @brief
///
struct DHeaderStructure final
{
  uint64_t mPlainBufferLength       = {};
  uint64_t mCompressedBufferLength  = {};
};

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy::zlib
{

std::string DyCompressString(_MIN_ const std::string& uncompressedString)
{
  ///
  /// @param buffer
  /// @param plainStringLength      '\0' included buffer length.
  /// @param compressedStringLength '\0' included buffer length.
  ///
  static auto CreateHeaderStructureTo = [](
      std::vector<unsigned char>& buffer,
      const uint64_t plainStringLength,
      const uint64_t compressedStringLength)
  {
    unsigned char* uncompressedStringLengthPtr{ (unsigned char*)(&plainStringLength) };
    for (auto i{ 0 }; i < 8; ++i)
    {
      buffer.emplace_back(*uncompressedStringLengthPtr);
      uncompressedStringLengthPtr++;
    }
    unsigned char* compressedStringLengthPtr{ (unsigned char*)(&compressedStringLength) };
    for (auto i{ 0 }; i < 8; ++i)
    {
      buffer.emplace_back(*compressedStringLengthPtr);
      compressedStringLengthPtr++;
    }
  };

  //!
  //! FunctionBody∨
  //!

  // Zlib struct
  auto compressStream{z_stream{}};
  compressStream.zalloc   = Z_NULL;
  compressStream.zfree    = Z_NULL;
  compressStream.opaque   = Z_NULL;

  // setup `uncompressedString` as the input.
  const auto uncompressedStringLength { static_cast<uint64_t>(uncompressedString.size() + 1) };
  compressStream.avail_in   = uncompressedStringLength; // WITH TERMINATOR
  compressStream.next_in    = (Bytef*)uncompressedString.data();

  auto compressedString {std::vector<unsigned char>(uncompressedStringLength)};
  compressStream.avail_out  = compressedString.size();
  compressStream.next_out   = (Bytef*)compressedString.data();

  // The actual compression work.
  deflateInit(&compressStream, Z_BEST_COMPRESSION);
  deflate(&compressStream, Z_FINISH);
  deflateEnd(&compressStream);

  // Clean up empty '\0' sequence from resultCompressedString.
  auto it = std::find_if_not(compressedString.rbegin(), compressedString.rend(),
      [](const unsigned char i) { return i == '\0'; });
  auto distanceFromStart {-std::distance(compressedString.rend(), it)};

  compressedString.erase(compressedString.cbegin() + distanceFromStart, compressedString.cend());
  compressedString.emplace_back('\0');

  /// Compressed String File Structure -----
  /// 0...4...8..12..16..................EOF
  /// [Plain ][Compre][Compressed Infor]0
  /// [Byte  ][sedByt][mation String...]0
  // Make structure.
  const auto compressedStringLength { static_cast<uint64_t>(compressStream.total_out + 1) };
  auto structureString { std::vector<unsigned char>() };
  structureString.reserve(16 + compressedStringLength);

  // Setup header structure.
  CreateHeaderStructureTo(structureString, uncompressedStringLength, compressedStringLength);

  // Finally, emplace compressed string to `structureString`.
  structureString.insert(structureString.end(), compressedString.cbegin(), compressedString.cend());
  return {structureString.begin(), structureString.end()};
}

std::string DyDecompressString(_MIN_ const std::string& compressedBuffer)
{
  static auto GetHeaderInformation = [](const std::string& buffer) -> DHeaderStructure
  {
    auto headerStructure {DHeaderStructure{}};
    auto* plainBufferPtr       { (unsigned char*) &headerStructure.mPlainBufferLength };
    auto* compressedBufferPtr  { (unsigned char*) &headerStructure.mCompressedBufferLength };

    for (auto i {0}; i < 8; ++i)
    {
      *plainBufferPtr = (unsigned char)buffer[4 + i];
      plainBufferPtr++;
    }

    for (auto i {0}; i < 8; ++i)
    {
      *compressedBufferPtr = (unsigned char)buffer[4 + 8 + i];
      compressedBufferPtr++;
    }

    return headerStructure;
  };

  //!
  //! FunctionBody∨
  //!

  // Zlib struct
  auto decompressStream{z_stream{}};
  decompressStream.zalloc     = Z_NULL;
  decompressStream.zfree      = Z_NULL;
  decompressStream.opaque     = Z_NULL;

  // Get Header structure from buffer.
  auto header {GetHeaderInformation(compressedBuffer)};

  // setup `compressedStrin` as the input.
  auto plainBuffer {std::vector<unsigned char>(header.mPlainBufferLength)};
  decompressStream.avail_in   = header.mCompressedBufferLength;
  decompressStream.next_in    = (Bytef*)(compressedBuffer.data() + sizeof(DHeaderStructure) + 4);
  decompressStream.avail_out  = header.mPlainBufferLength;
  decompressStream.next_out   = (Bytef*)plainBuffer.data();

  // The actual decompression work.
  inflateInit(&decompressStream);
  inflate(&decompressStream, Z_NO_FLUSH);
  inflateEnd(&decompressStream);

  return {plainBuffer.cbegin(), plainBuffer.cend()};
}

} /// ::dy::zlib namespace