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
#include <Dy/Management/Type/FontResourceContainer.h>
#include <Dy/Helper/HelperZlib.h>
#include <nlohmann/json.hpp>

namespace dy
{

FDyFontResourceContainer::FDyFontResourceContainer(const PDyMetaFontInformation& fontInformation)
{
  FILE* fpRes = std::fopen(fontInformation.mFontInformationPath.c_str(), "r");
  MDY_ASSERT(fpRes != nullptr, "fpRes must not be nullptr.");

  std::fseek(fpRes, 0, SEEK_END);
  auto fileSize = std::ftell(fpRes);
  std::fseek(fpRes, 0, SEEK_SET);

  std::vector<TU08> buffer(fileSize);
  std::fread(buffer.data(), sizeof(TU08), buffer.size(), fpRes);

  std::fclose(fpRes);
  const auto uncompressedString = zlib::DyDecompressString({buffer.begin(), buffer.end()});

  const auto jsonAtlas = nlohmann::json::parse(uncompressedString);
  MDY_NOT_IMPLEMENTED_ASSERT();
}

FDyFontResourceContainer::~FDyFontResourceContainer()
{

}

} /// ::dy namespace