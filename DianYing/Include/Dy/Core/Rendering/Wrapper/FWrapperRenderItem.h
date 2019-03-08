#ifndef GUARD_DY_CORE_RENDERING_WRAPPER_FWrapperRenderItem_H
#define GUARD_DY_CORE_RENDERING_WRAPPER_FWrapperRenderItem_H
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

#include <Dy/Helper/Internal/FProxyHandler.h>
#include <Dy/Core/Rendering/Interface/IRenderItem.h>

//!
//! Forward declaration
//!

namespace dy
{
struct PDyRenderItemInstanceMetaInfo;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

class FWrapperRenderItem final : public FProxyHandler<IRenderItem>
{
public:
  using TSuper = FProxyHandler<IRenderItem>;

  explicit FWrapperRenderItem(const PDyRenderItemInstanceMetaInfo& iMetaInfo);

  virtual ~FWrapperRenderItem() = default;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RENDERING_WRAPPER_FWrapperRenderItem_H