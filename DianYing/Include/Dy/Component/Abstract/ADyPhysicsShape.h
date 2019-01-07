#ifndef GUARD_DY_COMPONENT_ABSTRACT_ADYPHYSICSSHAPE_H
#define GUARD_DY_COMPONENT_ABSTRACT_ADYPHYSICSSHAPE_H
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

/// Header file
#include <Dy/Element/Abstract/ADyGeneralBaseComponent.h>

namespace dy
{

///
/// @class ADyPhysicsShape
/// @brief
/// @TODO SCRIPT THIS
///
MDY_ABSTRACT ADyPhysicsShape : public ADyGeneralBaseComponent
{
public:
  ADyPhysicsShape(FDyActor& actorReference);
  virtual ~ADyPhysicsShape() = default;

  ADyPhysicsShape(const ADyPhysicsShape&)                                 = delete;
  ADyPhysicsShape& operator=(const ADyPhysicsShape&)                      = delete;
  ADyPhysicsShape(ADyPhysicsShape&& instance)                   noexcept  = default;
  ADyPhysicsShape& operator=(ADyPhysicsShape&& instance)        noexcept  = default;

  MDY_SET_TYPEMATCH_FUNCTION(::dy::ADyGeneralBaseComponent, ADyPhysicsShape);
  MDY_SET_CRC32_HASH_WITH_TYPE(ADyPhysicsShape);
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_ABSTRACT_ADYPHYSICSSHAPE_H