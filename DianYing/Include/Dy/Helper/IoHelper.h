#ifndef GUARD_DY_HELPER_IO_HELPER_H
#define GUARD_DY_HELPER_IO_HELPER_H
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

namespace dy
{

///
/// @brief
///
std::optional<std::vector<char>> DyReadBinaryFileAll(const std::string& filePath) {
  std::ifstream fileStream { filePath, std::ios::ate | std::ios::binary };
  if (!fileStream.is_open()) return std::nullopt;

  const size_t fileSize = static_cast<size_t>(fileStream.tellg());
  std::vector<char> fileBuffer(fileSize + 1);

  fileStream.seekg(0);
  fileStream.read(fileBuffer.data(), fileSize);

  fileStream.close();
  return fileBuffer;
}



} /// ::dy namespace

#endif /// GUARD_DY_HELPER_IO_HELPER_H