#ifndef GUARD_DY_COMPONENT_ABSTRACT_ADYPHYSICSRIGIDBODY_H
#define GUARD_DY_COMPONENT_ABSTRACT_ADYPHYSICSRIGIDBODY_H
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
/// @class ADyPhysicsRigidbody
/// @brief
/// @TODO SCRIPT THIS.
///
MDY_ABSTRACT ADyPhysicsRigidbody : public ADyGeneralBaseComponent
{
public:
  ADyPhysicsRigidbody(FDyActor& actorReference);
  virtual ~ADyPhysicsRigidbody() = default;

  ADyPhysicsRigidbody(const ADyPhysicsRigidbody&)                                 = delete;
  ADyPhysicsRigidbody& operator=(const ADyPhysicsRigidbody&)                      = delete;
  ADyPhysicsRigidbody(ADyPhysicsRigidbody&& instance)                   noexcept  = default;
  ADyPhysicsRigidbody& operator=(ADyPhysicsRigidbody&& instance)        noexcept  = default;

  MDY_SET_TYPEMATCH_FUNCTION(::dy::ADyGeneralBaseComponent, ADyPhysicsRigidbody);
  MDY_SET_CRC32_HASH_WITH_TYPE(ADyPhysicsRigidbody);
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_ABSTRACT_ADYPHYSICSRIGIDBODY_H