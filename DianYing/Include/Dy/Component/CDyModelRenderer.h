#ifndef GUARD_DY_COMPONENT_CDYMODELRENDERER_H
#define GUARD_DY_COMPONENT_CDYMODELRENDERER_H
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

#include <Dy/Element/Abstract/ADyGeneralBaseComponent.h>

namespace dy
{

///
/// @class CDyModelRenderer
/// @brief
///
class CDyModelRenderer final : public ADyGeneralBaseComponent
{
public:
  CDyModelRenderer(FDyActor& actorReference);
  virtual ~CDyModelRenderer() = default;

  CDyModelRenderer(const CDyModelRenderer&)                                 = delete;
  CDyModelRenderer& operator=(const CDyModelRenderer&)                      = delete;
  CDyModelRenderer(CDyModelRenderer&& instance)                   noexcept  = default;
  CDyModelRenderer& operator=(CDyModelRenderer&& instance)        noexcept  = default;

  MDY_SET_TYPEMATCH_FUNCTION(::dy::ADyGeneralBaseComponent, CDyModelRenderer);
  MDY_SET_CRC32_HASH_WITH_TYPE(CDyModelRenderer);
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_CDYMODELRENDERER_H