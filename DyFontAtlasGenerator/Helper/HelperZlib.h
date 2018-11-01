#ifndef GUARD_DY_HELPER_HELPERZLIB_H
#define GUARD_DY_HELPER_HELPERZLIB_H

#include <string>

namespace dy
{
namespace zlib
{

///
/// @brief  Compress uncompressed plain string to compressed string.
/// @param  uncompressedString
/// @return compressed string.
///
std::string CompressString(const std::string& uncompressedString);

///
/// @brief  Decompress compressed string to plain string.
/// @param  compressedString
/// @return plain string which have been decompressed.
///
std::string DecompressString(const std::string& compressedString);

} /// ::dy::zlib namespace
} /// ::dy       namespace

#endif /// GUARD_DY_HELPER_HELPERZLIB_H