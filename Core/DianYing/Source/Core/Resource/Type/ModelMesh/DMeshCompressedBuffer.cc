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
#include <Dy/Core/Resource/Type/ModelMesh/DMeshCompressedBuffer.h>
#include <Dy/Helper/System/Assertion.h>
#include <Dy/Helper/Library/HelperFilesystem.h>

namespace dy
{

DMeshCompressedBuffer DyCreateMeshBufferInfoFromDyMeshFile(const std::string& iFilePath)
{
  MDY_ASSERT_MSG_FORCE(
    IsFileExist(iFilePath) == true, 
    "Model external file path must be valid. Could not found file path.");

  FILE* fdFile = fopen(iFilePath.c_str(), "r");
  MDY_ASSERT_MSG_FORCE(fdFile != nullptr, "Failed to read file.");

  DMeshCompressedBuffer buffer;
  { // mRawBufferBytes
    const auto byte = std::fread(&buffer.mRawBufferBytes, sizeof(buffer.mRawBufferBytes), 1, fdFile);
    MDY_ASSERT_MSG_FORCE(byte == 1, "Failed to read file properly.");
  }
  { // mCompressedBufferBytes
    const auto byte = std::fread(&buffer.mCompressedBufferBytes, sizeof(buffer.mCompressedBufferBytes), 1, fdFile);
    MDY_ASSERT_MSG_FORCE(byte == 1, "Failed to read file properly.");
  }
  { // mCompressedBuffer
    buffer.mCompressedBuffer.resize(buffer.mCompressedBufferBytes);

    const auto byte = std::fread(buffer.mCompressedBuffer.data(), sizeof(char), buffer.mCompressedBufferBytes, fdFile);
    MDY_ASSERT_MSG_FORCE(byte == buffer.mCompressedBufferBytes, "Failed to read file properly.");
  }

  fclose(fdFile);
  return buffer;
}

} /// ::dy namespace