#ifndef GUARD_DY_CORE_RESOURCE_TYPE_MODELMESH_DDyMeshCompressedBuffer_H
#define GUARD_DY_CORE_RESOURCE_TYPE_MODELMESH_DDyMeshCompressedBuffer_H
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

namespace dy
{

/// @struct DMeshCompressedBuffer
/// @brief Compressed mesh information buffer. Used to getting mesh informations from side tool.
struct DMeshCompressedBuffer final
{
  // Must contains last `EOF` ('\0').
  unsigned mRawBufferBytes;
  // 4Byte
  unsigned mCompressedBufferBytes;
  // ?Byte. (mCompressedBufferBytes)
  std::vector<char> mCompressedBuffer;
};

/// @brief Create `DMeshCompressedBuffer` information from `dyMesh` file.
MDY_NODISCARD DMeshCompressedBuffer 
DyCreateMeshBufferInfoFromDyMeshFile(const std::string& iFilePath);

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_TYPE_MODELMESH_DDyMeshCompressedBuffer_H
