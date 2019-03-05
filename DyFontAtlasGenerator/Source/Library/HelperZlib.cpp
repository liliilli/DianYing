#include <precompiled.h>
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

/// Header file
#include <Include/Library/HelperZlib.h>

#include <cstdlib>
#include <QByteArray>
#include <zlib/zlib.h>
#include <nlohmann/json.hpp>

//!
//! Forward declaration & local translation unit function
//!

namespace
{

/// @struct DHeaderStructure
/// @brief
struct DHeaderStructure final
{
  uint64_t mPlainBufferLength {};
  uint64_t mCompressedBufferLength {};
};

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy::zlib
{

DZlibResult CompressBuffer(const char* plainBuffer, const uint64_t bufferSize)
{
  // Zlib struct
  auto compressStream{ z_stream{} };
  compressStream.zalloc = Z_NULL;
  compressStream.zfree = Z_NULL;
  compressStream.opaque = Z_NULL;

  // set up the input.
  const auto decompressedLength = bufferSize;
  compressStream.avail_in = decompressedLength; // WITH TERMINATOR
  compressStream.next_in = (Bytef*)plainBuffer;

  std::vector<char> compressedBuffer(bufferSize);
  memcpy(compressedBuffer.data(), plainBuffer, bufferSize);
  compressStream.avail_out = compressedBuffer.size();
  compressStream.next_out = (Bytef*)compressedBuffer.data();

  // The actual compression work.
  deflateInit(&compressStream, Z_BEST_COMPRESSION);
  deflate(&compressStream, Z_FINISH);
  deflateEnd(&compressStream);

  // Clean up empty '\0' sequence from resultCompressedString.
  const auto compressedLength = static_cast<uint64_t>(compressStream.total_out);
  const auto it = std::find_if_not(compressedBuffer.rbegin(), compressedBuffer.rend(), [](const unsigned char i) { return i == '\0'; });
  const auto distanceFromStart{ -std::distance(compressedBuffer.rend(), it) };
  compressedBuffer.resize(compressedLength);

  return DZlibResult{ compressedLength , decompressedLength , compressedBuffer };
}

DZlibResult CompressBuffer(const QByteArray& plainBuffer)
{
  return CompressBuffer(plainBuffer.constData(), plainBuffer.size());
}

DZlibResult CompressBuffer(const std::string& plainBuffer)
{
  return CompressBuffer(plainBuffer.c_str(), plainBuffer.size());
}

DZlibResult CompressBuffer(const nlohmann::json& plainBuffer)
{
  return CompressBuffer(plainBuffer.dump(0));
}

std::vector<char> DecompressString(const DZlibResult& compressedStructure)
{
  // Zlib struct
  auto decompressStream{ z_stream{} };
  decompressStream.zalloc = Z_NULL;
  decompressStream.zfree  = Z_NULL;
  decompressStream.opaque = Z_NULL;

  // setup `compressedStrin` as the input.
  std::vector<char> plainBuffer(compressedStructure.mDecompressedLength);
  decompressStream.avail_in   = compressedStructure.mCompressedLength;
  decompressStream.next_in    = (Bytef*)compressedStructure.mCompressedBuffer.data();
  decompressStream.avail_out  = compressedStructure.mDecompressedLength;
  decompressStream.next_out   = (Bytef*)plainBuffer.data();

  // The actual decompression work.
  inflateInit(&decompressStream);
  inflate(&decompressStream, Z_NO_FLUSH);
  inflateEnd(&decompressStream);
  return plainBuffer;
}

std::string CompressString_Deprecated(const std::string& uncompressedString)
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

std::string DecompressString_Deprecated(const std::string& compressedBuffer)
{
  static auto GetHeaderInformation = [](const std::string& buffer) -> DHeaderStructure
  {
    auto headerStructure {DHeaderStructure{}};
    auto* plainBufferPtr       { (unsigned char*) &headerStructure.mPlainBufferLength };
    auto* compressedBufferPtr  { (unsigned char*) &headerStructure.mCompressedBufferLength };

    for (auto i {0}; i < 8; ++i)
    {
      *plainBufferPtr = (unsigned char)buffer[i];
      plainBufferPtr++;
    }

    for (auto i {0}; i < 8; ++i)
    {
      *compressedBufferPtr = (unsigned char)buffer[8 + i];
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
  decompressStream.next_in    = (Bytef*)(compressedBuffer.data() + sizeof(DHeaderStructure));
  decompressStream.avail_out  = header.mPlainBufferLength;
  decompressStream.next_out   = (Bytef*)plainBuffer.data();

  // The actual decompression work.
  inflateInit(&decompressStream);
  inflate(&decompressStream, Z_NO_FLUSH);
  inflateEnd(&decompressStream);

  return {plainBuffer.cbegin(), plainBuffer.cend()};
}

} /// ::dy::zlib namespace