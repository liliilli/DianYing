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
#include <Dy/Management/Internal/Font/FFontContainerBuiltin.h>
#include <Dy/Helper/Library/HelperZlib.h>
#include <nlohmann/json.hpp>

#include <Dy/Helper/Internal/DImageBinaryBuffer.h>

namespace dy
{

FFontContainerBuiltin::FFontContainerBuiltin(const PDyMetaFontInformation::DBuiltin& details)
  : AFontContainer{
    PDyMetaFontInformation::ELoadingType::Builtin,
    details.mFontType}
{
  // (1) Open file and get plain information buffer.
  using DBuiltin  = PDyMetaFontInformation::DBuiltin;
  using EBuffer   = PDyMetaFontInformation::DBuiltin::EBuffer;

  nlohmann::json jsonAtlas;
  using XInfo = DBuiltin::XInfo;
  switch (details.mFontInfoType)
  {
  case EBuffer::Plain: 
  {
    auto buffer = std::get<XInfo::ToDetailType<EBuffer::Plain>>(details.mFontInfoBuffer);
    jsonAtlas = nlohmann::json::parse(buffer);
  } break;
  case EBuffer::Index: 
  {
    MDY_NOT_IMPLEMENTED_ASSERT();
  } break;
  default: MDY_UNEXPECTED_BRANCH(); break;
  }

  // (2) Make character information.
  const nlohmann::json& characterAtlas = jsonAtlas["Characters"];
  for (auto objIterator = characterAtlas.begin(); objIterator != characterAtlas.end(); ++objIterator)
  {
    const TChr16 charCode = std::stoi(objIterator.key());
    MDY_ASSERT(this->mCharContainer.find(charCode) == this->mCharContainer.end());

    auto [_, isSucceeded] = mCharContainer.try_emplace(
        charCode,
        DDyFontCharacterInfo::CreateInstance(objIterator.value(), charCode));
    MDY_ASSERT_MSG(isSucceeded == true, "Unexpected error occurred.");
  }

  // (3) Allocate GL2DArray for font textures.
  // * 1. Read .png s and make them as serialized buffer and TextureResource, depends on this resource.
  // * 2. Get resource from TextureResource.
  using XTexture = DBuiltin::XTexture;
  switch (details.mTextureType)
  {
  case EBuffer::Plain: 
  {
    MDY_NOT_IMPLEMENTED_ASSERT();
  } break;
  case EBuffer::Index: 
  {
    const auto& list = std::get<XTexture::ToDetailType<EBuffer::Index>>(details.mTexureBuffers);
    const auto count = static_cast<TI32>(list.size());

    glGenTextures (1, &this->mTexImageResId);
    glBindTexture (GL_TEXTURE_2D_ARRAY, this->mTexImageResId);
    glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, 1024, 1024, count);
    for (TI32 i = 0; i < count; ++i)
    {
      std::size_t resourceSize = 0;
      void* ptr = nullptr;
#ifdef _WIN32
      /// https://stackoverflow.com/questions/29461310/findresource-giving-error-1813-when-loading-png
      HRSRC hResource = FindResource(nullptr, MAKEINTRESOURCE(list[i]), L"PNG");
      MDY_ASSERT_FORCE(hResource != nullptr);

      HGLOBAL hMemory = LoadResource(nullptr, hResource);
      resourceSize = SizeofResource(nullptr, hResource);
      ptr = LockResource(hMemory);
#endif
      const auto dataBuffer = std::make_unique<DImageBinaryBuffer>(resourceSize, ptr);
      MDY_ASSERT(dataBuffer->IsBufferCreatedProperly() == true);

      glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 
        0, 0, 0, i, 1024, 1024, 1, 
        GL_RGBA, GL_UNSIGNED_BYTE, 
        dataBuffer->GetBufferStartPoint());
#ifdef _WIN32
      FreeResource(hMemory);
#endif
    }
  } break;
  default: MDY_UNEXPECTED_BRANCH(); break;
  }

  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glBindTexture (GL_TEXTURE_2D_ARRAY, 0);
}

FFontContainerBuiltin::~FFontContainerBuiltin()
{
  glDeleteTextures(1, &this->mTexImageResId);
}

bool FFontContainerBuiltin::IsCharacterGlyphExist(TChr16 fontCode)
{
  return this->mCharContainer.find(fontCode) != this->mCharContainer.end();
}

const DDyFontCharacterInfo& FFontContainerBuiltin::GetGlyphCharacter(TChr16 fontCode)
{
  if (this->IsCharacterGlyphExist(fontCode) == true)  { return this->mCharContainer[fontCode]; }
  else                                                { return sDefaultFontCharacterInfo; }
}

TI32 FFontContainerBuiltin::GetLinefeedHeight(TI32 fontSize) const noexcept
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  return 0;
}

const DDyFontCharacterInfo& FFontContainerBuiltin::operator[](TChr16 fontCode)
{
  return this->mCharContainer[fontCode];
}

TI32 FFontContainerBuiltin::GetFontTextureArrayId() const noexcept
{
  return this->mTexImageResId;
}

} /// ::dy namespace