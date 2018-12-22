#ifndef GUARD_DY_HELPER_HELPERZLIB_H
#define GUARD_DY_HELPER_HELPERZLIB_H

#include <string>
#include <vector>
#include <nlohmann/json_fwd.hpp>

class QByteArray;

namespace dy
{
namespace zlib
{

struct DZlibResult
{
  uint64_t mCompressedLength          = {};
  uint64_t mDecompressedLength        = {};
  std::vector<char> mCompressedBuffer = {};

  DZlibResult() = default;
  DZlibResult(uint64_t compressedLength, uint64_t decompressedLength, const std::vector<char>& compressedBuffer)
    : mCompressedLength(compressedLength),
      mDecompressedLength(decompressedLength),
      mCompressedBuffer(compressedBuffer)
  {}
};

///
/// @brief  Compress uncompressed plain string to compressed string.
/// @return compressed string which wrapped by result `DZlibResult` instance.
///
[[nodiscard]] DZlibResult CompressBuffer(const char* plainBuffer, const uint64_t bufferSize);
[[nodiscard]] DZlibResult CompressBuffer(const QByteArray& plainBuffer);
[[nodiscard]] DZlibResult CompressBuffer(const std::string& plainBuffer);
[[nodiscard]] DZlibResult CompressBuffer(const nlohmann::json& plainBuffer);

///
/// @brief  Decompress compressed structure which has a `zlib`ed buffer to plain buffer.
/// @return plain buffer which have been decompressed.
///
[[nodiscard]] std::vector<char> DecompressString(const DZlibResult& compressedStructure);

///
/// @brief  Compress uncompressed plain string to compressed string.
/// @param  uncompressedString
/// @return compressed string.
///
std::string CompressString_Deprecated(const std::string& uncompressedString);

///
/// @brief  Decompress compressed string to plain string.
/// @param  compressedString
/// @return plain string which have been decompressed.
///
std::string DecompressString_Deprecated(const std::string& compressedString);

} /// ::dy::zlib namespace
} /// ::dy       namespace

#endif /// GUARD_DY_HELPER_HELPERZLIB_H