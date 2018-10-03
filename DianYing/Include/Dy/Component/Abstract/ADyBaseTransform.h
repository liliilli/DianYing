#ifndef GUARD_DY_COMPONENT_ABSTRACT_ADYBASETRANSFORM_H
#define GUARD_DY_COMPONENT_ABSTRACT_ADYBASETRANSFORM_H
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

#include <Dy/Component/Descriptor/ComponentMetaDescriptor.h>
#include <Dy/Element/Abstract/ADyGeneralBaseComponent.h>

namespace dy
{

///
/// @class ADyBaseTransform
/// @brief
/// @TODO SCRIPT THIS
///
MDY_ABSTRACT ADyBaseTransform : public ADyGeneralBaseComponent
{
public:
  ADyBaseTransform(FDyActor& actorReference);
  virtual ~ADyBaseTransform() = default;

  ///
  MDY_NODISCARD EDySuccess Initialize(_MIN_ const DDyTransformMetaInformation& desc);

  ///
  void Release();

  ADyBaseTransform(const ADyBaseTransform&)                                 = delete;
  ADyBaseTransform& operator=(const ADyBaseTransform&)                      = delete;
  ADyBaseTransform(ADyBaseTransform&& instance)                   noexcept  = default;
  ADyBaseTransform& operator=(ADyBaseTransform&& instance)        noexcept  = default;

  MDY_SET_TYPEMATCH_FUNCTION(::dy::ADyGeneralBaseComponent, ADyBaseTransform);
  MDY_SET_CRC32_HASH_WITH_TYPE(ADyBaseTransform);
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_ABSTRACT_ADYBASETRANSFORM_H