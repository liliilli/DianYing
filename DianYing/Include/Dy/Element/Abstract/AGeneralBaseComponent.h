#ifndef GUARD_DY_ELEMENT_ABSTRACT_AGeneralBaseComponent_H
#define GUARD_DY_ELEMENT_ABSTRACT_AGeneralBaseComponent_H
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

#include <Dy/Element/Abstract/ADyBaseComponent.h>
#include <Dy/Element/Interface/IUpdatable.h>

namespace dy
{

/// @class AGeneralBaseComponent
/// @brief General base component except for script component.
MDY_ABSTRACT AGeneralBaseComponent : public ADyBaseComponent, public IUpdatable
{
public:
  AGeneralBaseComponent(FDyActor& actorReference);
  virtual ~AGeneralBaseComponent() = default;

  AGeneralBaseComponent(const AGeneralBaseComponent&)                           = delete;
  AGeneralBaseComponent& operator=(const AGeneralBaseComponent&)                = delete;
  AGeneralBaseComponent(AGeneralBaseComponent&& instance)             noexcept  = default;
  AGeneralBaseComponent& operator=(AGeneralBaseComponent&& instance)  noexcept  = default;

  MDY_SET_TYPEMATCH_FUNCTION(::dy::ADyBaseComponent, AGeneralBaseComponent);
  MDY_SET_CRC32_HASH_WITH_TYPE(AGeneralBaseComponent);
};

} /// ::dy namespace

#endif /// GUARD_DY_ELEMENT_ABSTRACT_AGeneralBaseComponent_H