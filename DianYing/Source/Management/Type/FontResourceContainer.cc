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
#include <Dy/Management/Type/FontResourceContainer.h>
#include <Dy/Helper/Library/HelperZlib.h>
#include <nlohmann/json.hpp>

#include <Dy/Helper/Internal/DImageBinaryBuffer.h>

//!
//! Local translation unit files
//!

namespace
{

dy::DDyFontCharacterInfo sDefaultFontCharacterInfo = {};

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy
{

FDyFontResourceContainer::FDyFontResourceContainer(const PDyMetaFontInformation& fontInformation)
{
  static auto GetPlainInformationString = [](const std::string& filePath) -> std::string
  {
    FILE* fpRes = std::fopen(filePath.c_str(), "rb");
    MDY_ASSERT_MSG(fpRes != nullptr, "fpRes must not be nullptr.");

    std::fseek(fpRes, 0, SEEK_END);
    auto fileSize = std::ftell(fpRes);
    std::fseek(fpRes, 0, SEEK_SET);

    std::vector<TU8> buffer(fileSize);
    std::fread(buffer.data(), sizeof(TU8), buffer.size(), fpRes);
    std::fclose(fpRes);

    return zlib::DecompressString({buffer.begin(), buffer.end()});
  };

  //! FUNCTIONBODY ∨
  MDY_NOT_IMPLEMENTED_ASSERT();

#ifdef false
  // (1) Open file and get plain information buffer.
  const auto plainInformationString = GetPlainInformationString(fontInformation.mFontInformationPath);
  const nlohmann::json jsonAtlas    = nlohmann::json::parse(plainInformationString);

  // (2) Make character information.
  const nlohmann::json& characterAtlas = jsonAtlas["Characters"];
  for (auto objIterator = characterAtlas.begin(); objIterator != characterAtlas.end(); ++objIterator)
  {
    const TChr16 charCode = std::stoi(objIterator.key());
    MDY_ASSERT_MSG(this->mCharContainer.find(charCode) == this->mCharContainer.end(), "Unexpected error occurred.");

    auto [_, isSucceeded] = mCharContainer.try_emplace(
        charCode,
        DDyFontCharacterInfo::CreateInstance(objIterator.value(), charCode));
    MDY_ASSERT_MSG(isSucceeded == true, "Unexpected error occurred.");
  }

  // (3) Allocate GL2DArray for font textures.
  // * 1. Read .png s and make them as serialized buffer and TextureResource, depends on this resource.
  // * 2. Get resource from TextureResource.
  // ** 지금은 그냥 무식하게 여기에다가 하는 것으로 하자.

  const auto texturePackSize = static_cast<TI32>(fontInformation.mFontTexturePathList.size());

  glGenTextures (1, &this->mTexImageResId);
  glBindTexture (GL_TEXTURE_2D_ARRAY, this->mTexImageResId);
  glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, 1024, 1024, texturePackSize);
  for (TI32 i = 0; i < texturePackSize; ++i)
  {
    auto dataBuffer = std::make_unique<DImageBinaryBuffer>(fontInformation.mFontTexturePathList[i]);
    MDY_ASSERT_MSG(dataBuffer->IsBufferCreatedProperly() == true, "Unexpected error occurred.");
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, 1024, 1024, 1, GL_RGBA, GL_UNSIGNED_BYTE, dataBuffer->GetBufferStartPoint());
  }
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glBindTexture (GL_TEXTURE_2D_ARRAY, 0);

  // (4) Set parameters
#endif
}

FDyFontResourceContainer::~FDyFontResourceContainer()
{
  glDeleteTextures(1, &this->mTexImageResId);
}

bool FDyFontResourceContainer::IsCharacterGlyphExist(const TChr16 fontCode)
{
  return this->mCharContainer.find(fontCode) != this->mCharContainer.end();
}

const DDyFontCharacterInfo& FDyFontResourceContainer::GetGlyphCharacter(const TChr16 fontCode)
{
  if (this->IsCharacterGlyphExist(fontCode) == true)  { return this->mCharContainer[fontCode]; }
  else                                                { return sDefaultFontCharacterInfo; }
}

TI32 FDyFontResourceContainer::GetLinefeedHeight(const TI32 fontSize) const noexcept
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  return 0;
}

const DDyFontCharacterInfo& FDyFontResourceContainer::operator[](const TChr16 fontCode)
{
  return this->mCharContainer[fontCode];
}

} /// ::dy namespace