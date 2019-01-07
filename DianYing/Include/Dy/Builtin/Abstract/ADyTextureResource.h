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

#include <Dy/Builtin/Interface/IDyResource.h>
#include <Dy/Core/Reflection/RDyBuiltinResources.h>
#include <Dy/Helper/Type/ColorRGBA32.h>
#include <Dy/Meta/Information/MetaInfoTexture.h>

namespace dy
{

struct ADyTextureResource : public IDyResource
{
protected:
  using TBufferType = std::vector<TU08>;
  /// Temporary texture meta info.
  PDyTextureInstanceMetaInfo mTextureMetaInfo{};
public:
  static constexpr auto value = EDyResourceType::Texture;

  /// Buffer pointer to bind using `GetMetaInfo()`.
  TBufferType* mPtrBuffer = MDY_INITIALIZE_NULL;

  /// @brief Get meta information that has buffer to texture and properties.
  std::any GetMetaInfo() override final;
};

template <int TN, int TV>
MDY_NODISCARD std::vector<TU08> ConvertToTU08VectorList(const std::array<std::array<TU08, TN>, TV>& buffer) noexcept
{
  std::vector<TU08> result {};
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
MDY_NODISCARD std::vector<TU08> ConvertToTU08VectorList(const std::array<dy::DDyColorRGBA32, TV>& buffer) noexcept
{
  std::vector<TU08> result {};
  result.reserve(TV * 4);

  for (int y = 0; y < TV; ++y)
  {
    const dy::DDyColorRGBA32& buf = buffer[y];
    result.emplace_back(buf.R);
    result.emplace_back(buf.G);
    result.emplace_back(buf.B);
    result.emplace_back(buf.A);
  }
  return std::move(result);
}

} /// ::dy namespace

#endif /// GUARD_DY_BUILTIN_ABSTRACT_ADYTEXTURERESOURCE_H
