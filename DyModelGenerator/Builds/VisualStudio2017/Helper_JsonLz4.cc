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
#include "Helper_JsonLz4.h"
#include <nlohmann/json.hpp>
#include <lz4.h>
#include "juce_core/system/juce_PlatformDefs.h"

void to_json(nlohmann::json& j, const DCompressedBuffer& p)
{
  j = nlohmann::json
  {
    {"b", p.mRawBufferBytes},
    {"c", p.mCompressedBufferBytes},
    {"d", std::string(p.mCompressedBuffer.begin(), p.mCompressedBuffer.end())},
  };
}

void from_json(const nlohmann::json& j, DCompressedBuffer& p)
{
  p.mRawBufferBytes         = j["b"].get<unsigned>();
  p.mCompressedBufferBytes  = j["c"].get<unsigned>();

  const auto str = j["d"].get<std::string>();
  p.mCompressedBuffer = decltype(p.mCompressedBuffer){str.begin(), str.end()};
}

DCompressedBuffer CompressStringBuffer(const std::string& iRawStringBuffer)
{
  const auto srcSize                = static_cast<unsigned>(iRawStringBuffer.length());
  const auto looseCompressedBtSize  = LZ4_compressBound(srcSize);

  DCompressedBuffer result;
  result.mCompressedBuffer.resize(looseCompressedBtSize);

  const int compressedBtSize = LZ4_compress_default(iRawStringBuffer.c_str(), result.mCompressedBuffer.data(), srcSize, looseCompressedBtSize);
  jassert(compressedBtSize > 0);

  result.mRawBufferBytes        = srcSize;
  result.mCompressedBufferBytes = compressedBtSize;
  result.mCompressedBuffer.resize(compressedBtSize);
  return result;
}

std::string DecompressLz4Buffer(const DCompressedBuffer& iBuffer)
{
  std::vector<char> rawChunk;
  rawChunk.resize(iBuffer.mRawBufferBytes);

  const int decompressedSize = LZ4_decompress_safe(iBuffer.mCompressedBuffer.data(), rawChunk.data(), iBuffer.mCompressedBufferBytes, iBuffer.mRawBufferBytes);
  jassert(decompressedSize > 0);

  return std::string{rawChunk.begin(), rawChunk.end()};
}
