#ifndef GUARD_DY_COMPONENT_INTERNAL_PHYSICS_FDyCollisionSignatureContainer_H
#define GUARD_DY_COMPONENT_INTERNAL_PHYSICS_FDyCollisionSignatureContainer_H
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

#include "CallbackSpecifier.h"
#include <Dy/Component/Interface/Physics/IDyonColliderBaseDelegator.h>

//!
//! Forward declaration
//!

namespace dy
{
class ADyActorCppScript;
class FDyonHitDelegatorContainer;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

/// @class FDyonHitDelegatorContainer
/// @brief
class FDyonHitDelegatorContainer final : public IDyonColliderBaseDelegator<TSignatureCollisionHit>
{
public:
  virtual ~FDyonHitDelegatorContainer() = default;
};

/// @class FDyCollisionSignatureContainer
/// @brief 
class FDyCollisionSignatureContainer final
{
public:
  FDyonHitDelegatorContainer onHit;
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_INTERNAL_PHYSICS_FDyCollisionSignatureContainer_H