#ifndef GUARD_DY_BUILTIN_ABSTRACT_ADYTEXTURERESOURCE_H
#define GUARD_DY_BUILTIN_ABSTRACT_ADYTEXTURERESOURCE_H
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

#include <Dy/Builtin/Interface/IDyResource.h>
#include <Dy/Core/Reflection/RDyBuiltinResources.h>
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

} /// ::dy namespace

#endif /// GUARD_DY_BUILTIN_ABSTRACT_ADYTEXTURERESOURCE_H
