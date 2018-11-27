#ifndef GUARD_DY_ELEMENT_ABSTRACT_ADYGENERALBASECOMPONENT_H
#define GUARD_DY_ELEMENT_ABSTRACT_ADYGENERALBASECOMPONENT_H
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

#include <Dy/Element/Abstract/ADyBaseComponent.h>
#include <Dy/Element/Interface/IDyUpdatable.h>

namespace dy
{

///
/// @class ADyGeneralBaseComponent
/// @brief General base component except for script component.
///
MDY_ABSTRACT ADyGeneralBaseComponent : public ADyBaseComponent, public IDyUpdatable
{
public:
  ADyGeneralBaseComponent(FDyActor& actorReference);
  virtual ~ADyGeneralBaseComponent() = default;

  ADyGeneralBaseComponent(const ADyGeneralBaseComponent&)                           = delete;
  ADyGeneralBaseComponent& operator=(const ADyGeneralBaseComponent&)                = delete;
  ADyGeneralBaseComponent(ADyGeneralBaseComponent&& instance)             noexcept  = default;
  ADyGeneralBaseComponent& operator=(ADyGeneralBaseComponent&& instance)  noexcept  = default;

  MDY_SET_TYPEMATCH_FUNCTION(::dy::ADyBaseComponent, ADyGeneralBaseComponent);
  MDY_SET_CRC32_HASH_WITH_TYPE(ADyGeneralBaseComponent);
};

} /// ::dy namespace

#endif /// GUARD_DY_ELEMENT_ABSTRACT_ADYGENERALBASECOMPONENT_H