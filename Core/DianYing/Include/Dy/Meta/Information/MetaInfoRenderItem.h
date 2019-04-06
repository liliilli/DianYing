#ifndef GUARD_DY_META_INFORMATION_PMetaInfoRenderItem_H
#define GUARD_DY_META_INFORMATION_PMetaInfoRenderItem_H
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

#include <Dy/Core/Rendering/Interface/IRenderItem.h>
#include <Dy/Meta/Information/CommonResourceMetaInfo.h>
#include <Dy/Helper/Type/DUuid.h>

namespace dy
{

/// @struct PDyRenderItemInstanceMetaInfo
/// @brief Meta information for constructing Rendering item instance.
struct PDyRenderItemInstanceMetaInfo final : public PDyCommonResourceMetaInfo
{
  /// @brief Name of render item.
  std::string mSpecifierName;
  /// @brief Uuid specifier. This value must be specified and not empty value.
  DUuid       mUuid{true};
  
  using TItemFunction = std::unique_ptr<IRenderItem>(*)();
  /// @brief do not touch, this will be handled automatically in Dy System.
  TItemFunction mBtInstantiationFunction = nullptr;
};

} /// ::dy namespace

#endif /// GUARD_DY_META_INFORMATION_PMetaInfoRenderItem_H