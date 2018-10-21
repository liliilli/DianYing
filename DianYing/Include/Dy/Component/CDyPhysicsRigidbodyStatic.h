#ifndef GUARD_DY_COMPONENT_CDYPHYSICSRIGIDBODYSTATIC_H
#define GUARD_DY_COMPONENT_CDYPHYSICSRIGIDBODYSTATIC_H
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

#include <Dy/Component/Abstract/ADyPhysicsRIgidbody.h>

namespace dy
{

///
/// @class CDyPhysicsRigidbodyStatic
/// @brief
///
class CDyPhysicsRigidbodyStatic final : public ADyPhysicsRigidbody
{
public:
  CDyPhysicsRigidbodyStatic(FDyActor& actorReference);
  virtual ~CDyPhysicsRigidbodyStatic() = default;

  CDyPhysicsRigidbodyStatic(const CDyPhysicsRigidbodyStatic&)                          = delete;
  CDyPhysicsRigidbodyStatic& operator=(const CDyPhysicsRigidbodyStatic&)               = delete;
  CDyPhysicsRigidbodyStatic(CDyPhysicsRigidbodyStatic&& instance)            noexcept  = default;
  CDyPhysicsRigidbodyStatic& operator=(CDyPhysicsRigidbodyStatic&& instance) noexcept  = default;

  MDY_SET_TYPEMATCH_FUNCTION(::dy::ADyPhysicsRigidbody, CDyPhysicsRigidbodyStatic);
  MDY_SET_CRC32_HASH_WITH_TYPE(CDyPhysicsRigidbodyStatic)
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_CDYPHYSICSRIGIDBODYSTATIC_H