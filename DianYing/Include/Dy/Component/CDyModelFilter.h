#ifndef GUARD_DY_COMPONENT_CDYMODELFILTER_H
#define GUARD_DY_COMPONENT_CDYMODELFILTER_H
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
/// @class CDyModelFilter
/// @brief
///
class CDyModelFilter final : public ADyGeneralBaseComponent
{
public:
  CDyModelFilter(FDyActor& actorReference);
  virtual ~CDyModelFilter() = default;

  CDyModelFilter(const CDyModelFilter&)                                 = delete;
  CDyModelFilter& operator=(const CDyModelFilter&)                      = delete;
  CDyModelFilter(CDyModelFilter&& instance)                   noexcept  = default;
  CDyModelFilter& operator=(CDyModelFilter&& instance)        noexcept  = default;

  MDY_SET_TYPEMATCH_FUNCTION(::dy::ADyGeneralBaseComponent, CDyModelFilter);
  MDY_SET_CRC32_HASH_WITH_TYPE(CDyModelFilter);
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_CDYMODELFILTER_H