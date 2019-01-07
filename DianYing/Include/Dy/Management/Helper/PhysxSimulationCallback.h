#ifndef GUARD_DY_MANAGEMENT_HELPER_PHYSX_SIMULATION_CALLBACK_H
#define GUARD_DY_MANAGEMENT_HELPER_PHYSX_SIMULATION_CALLBACK_H
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

#include <PxSimulationEventCallback.h>

namespace dy
{

///
/// @class FDyPxSimulationEventCallback
/// @brief
///
class FDyPxSimulationEventCallback final : public physx::PxSimulationEventCallback
{
public:
  ///
  /// @brief
  /// @param
  /// @param
  /// @param
  ///
  void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) override final;

  ///
  /// @brief
  /// @param
  /// @param
  /// @param
  ///
  void onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count) override final;

  ///
  /// @brief
  /// @param
  /// @param
  ///
  void onSleep(physx::PxActor** actors, physx::PxU32 count) override final;

  ///
  /// @brief
  /// @param
  /// @param
  ///
  void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override final;

  ///
  /// @brief
  /// @param
  /// @param
  ///
  void onWake(physx::PxActor** actors, physx::PxU32 count) override final;

  ///
  /// @brief
  /// @param
  /// @param
  ///
  void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) override final;
};

}

#endif /// GUARD_DY_MANAGEMENT_HELPER_PHYSX_SIMULATION_CALLBACK_H