#ifndef GUARD_DY_BUILTIN_ABSTRACT_ADYTEXTURERESOURCE_H
#define GUARD_DY_BUILTIN_ABSTRACT_ADYTEXTURERESOURCE_H
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

#include <Dy/Builtin/Interface/IResource.h>
#include <Dy/Core/Reflection/RBuiltinResources.h>
#include <Dy/Helper/Type/DColorRGBA32.h>
#include <Dy/Meta/Information/MetaInfoTexture.h>

namespace dy
{

/// @macro MDY_REGISTER_RESOURCE_TEXTURE
/// @brief Register texture meta information source.
#define MDY_REGISTER_RESOURCE_TEXTURE(__MAType__, __MASpecifierName__) \
  MDY_REGISTER_RESOURCE_WITH_SPECIFIER(__MAType__, __MASpecifierName__) \
  private: \
  static TBufferType& __Get() noexcept \
  { \
    static TBufferType instance{}; \
    return instance; \
  } \
  void ConstructBuffer(_MOUT_ TBufferType& buffer, _MOUT_ PDyTextureInstanceMetaInfo& property) noexcept; \
  public: \
  __MAType__() \
  { \
    ConstructBuffer(__Get(), this->mTextureMetaInfo); \
    this->mPtrBuffer = &__Get(); \
  }

struct ATextureResource : public IResource
{
protected:
  /// Temporary texture meta info.
  PDyTextureInstanceMetaInfo mTextureMetaInfo{};
public:
  using TBufferType = std::vector<TU8>;
  static constexpr auto value = EResourceType::Texture;

  /// Buffer pointer to bind using `GetMetaInfo()`.
  TBufferType* mPtrBuffer = MDY_INITIALIZE_NULL;

  /// @brief Get meta information that has buffer to texture and properties.
  std::any GetMetaInfo() override final;
};

template <int TN, int TV>
MDY_NODISCARD std::vector<TU8> ConvertToTU08VectorList(const std::array<std::array<TU8, TN>, TV>& buffer) noexcept
{
  std::vector<TU8> result {};
  for (int y = 0; y < TV; ++y)
  {
    for (int x = 0; x < TN; ++x)
    {
      result.emplace_back(buffer[y][x]);
    }
  }
  return std::move(result);
}

template <int TV>
MDY_NODISCARD std::vector<TU8> ConvertToTU08VectorList(const std::array<dy::DColorRGBA32, TV>& buffer) noexcept
{
  std::vector<TU8> result {};
  result.reserve(TV * 4);

  for (int y = 0; y < TV; ++y)
  {
    const dy::DColorRGBA32& buf = buffer[y];
    result.emplace_back(buf.R);
    result.emplace_back(buf.G);
    result.emplace_back(buf.B);
    result.emplace_back(buf.A);
  }
  return std::move(result);
}

} /// ::dy namespace

#endif /// GUARD_DY_BUILTIN_ABSTRACT_ADYTEXTURERESOURCE_H
