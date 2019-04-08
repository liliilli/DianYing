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
#include <Dy/Core/Rendering/Wrapper/XGLWrapper.h>
#include <Dy/Core/Rendering/Wrapper/PDyGLTextureDescriptor.h>

#include <Dy/Include/GlInclude.h>
#include <AEngineBase.h>

#ifdef _WIN32
#undef FindResource
#endif

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
    const auto infoId = std::get<XInfo::ToDetailType<EBuffer::Index>>(details.mFontInfoBuffer);

    // Find json information
    auto& platformInfo = gEngine->GetPlatformInfo();
    auto ptrResource = platformInfo.FindResource(infoId, EBtResource::JSON);
    MDY_ASSERT_FORCE(ptrResource != nullptr);

    // Parse
    jsonAtlas = nlohmann::json::parse(static_cast<const char*>(ptrResource->GetBufferPtr()));
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
      // Find texture information
      auto& platformInfo = gEngine->GetPlatformInfo();
      auto ptrResource = platformInfo.FindResource(list[i], EBtResource::PNG);
      MDY_ASSERT_FORCE(ptrResource != nullptr);

      // Get image buffer from texture information.
      std::size_t resourceSize = ptrResource->GetResourceSize();
      void* ptr = ptrResource->GetBufferPtr();
      const auto dataBuffer = std::make_unique<DImageBinaryBuffer>(resourceSize, ptr);
      MDY_ASSERT(dataBuffer->IsBufferCreatedProperly() == true);

      // Set image.
      glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 
        0, 0, 0, i, 1024, 1024, 1, 
        GL_RGBA, GL_UNSIGNED_BYTE, 
        dataBuffer->GetBufferStartPoint());
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