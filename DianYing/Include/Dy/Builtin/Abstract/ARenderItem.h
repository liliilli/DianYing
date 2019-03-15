#ifndef GUARD_DY_BUILTIN_ABSTRACT_ARENDERITEM_H
#define GUARD_DY_BUILTIN_ABSTRACT_ARENDERITEM_H
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
#include <Dy/Meta/Type/EResourceType.h>
#include <Dy/Core/Rendering/Interface/IRenderItem.h>

namespace dy
{

/// @macro MDY_REGISTER_RESOURCE_RENDER_ITEM
/// @brief Register rendering item into dy rendering System.
/// @param __MAType__ Type for being enabled.
#define MDY_REGISTER_RESOURCE_RENDER_ITEM(__MAType__, __MASpecifier__) \
  MDY_REGISTER_RESOURCE_WITH_SPECIFIER(__MAType__, __MASpecifier__); \
public: \
  class __ConstructionHelper final : public ::dy::IResource \
  { \
    using TItemFunction = ::dy::PDyRenderItemInstanceMetaInfo::TItemFunction; \
    using TFunctionReturn = std::invoke_result_t<TItemFunction>; \
    \
    template<typename TType> \
    static TFunctionReturn GetInstance() { return std::make_unique<TType>(); } \
    inline static auto function = GetInstance<__MAType__>; \
    \
    void ConstructBuffer(PDyRenderItemInstanceMetaInfo& oMeta); \
    std::any GetMetaInfo() override final \
    { \
      PDyRenderItemInstanceMetaInfo metaInfo; \
      metaInfo.mSpecifierName = sName; \
      this->ConstructBuffer(metaInfo); \
      metaInfo.mBtInstantiationFunction = function; \
      return metaInfo; \
    } \
  }; \
private:

/// @struct ARenderItem
/// @brief Item for rendering each pass.
/// Rendering must be held in these items.
struct ARenderItem : public IResource, IRenderItem
{
public:
  static constexpr auto value = EResourceType::RenderItem;

private:
  std::any GetMetaInfo() override final { return 0; };
};

} /// ::dy namespace

#endif /// !GUARD_DY_BUILTIN_ABSTRACT_ARENDERITEM_H
