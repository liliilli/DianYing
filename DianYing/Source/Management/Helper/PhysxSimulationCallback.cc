#include <precompiled.h>
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

/// Header field
#include <Dy/Management/Helper/PhysxSimulationCallback.h>

#include <PxRigidActor.h>
#include <Dy/Management/LoggingManager.h>

namespace dy
{

void FDyPxSimulationEventCallback::onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs)
{
  for (physx::PxU32 i = 0; i < nbPairs; ++i)
  {
    const physx::PxContactPair& cp = pairs[i];
    if (cp.events & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
    {
      std::string_view leftActorName  = pairHeader.actors[0]->getName();
      std::string_view rightActorName = pairHeader.actors[1]->getName();
#ifdef false
      pairHeader.actors[0]->userData
#endif
      MDY_LOG_CRITICAL("onContact | Left : {}, Right : {}", leftActorName.data(), rightActorName.data());
    }
  }
#ifdef false
  for(physx::PxU32 i=0; i < nbPairs; i++)
	{
		const physx::PxContactPair& cp = pairs[i];

		if(cp.events & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			if((pairHeader.actors[0] == mSubmarineActor) || (pairHeader.actors[1] == mSubmarineActor))
			{
			  physx::PxActor* otherActor = (mSubmarineActor == pairHeader.actors[0]) ? pairHeader.actors[1] : pairHeader.actors[0];
				Seamine* mine =  reinterpret_cast<Seamine*>(otherActor->userData);
				// insert only once
				if(std::find(mMinesToExplode.begin(), mMinesToExplode.end(), mine) == mMinesToExplode.end())
					mMinesToExplode.push_back(mine);

				break;
			}
		}
	}
#endif
}

void FDyPxSimulationEventCallback::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count)
{
#ifdef false
  for(PxU32 i=0; i < count; i++)
	{
		// ignore pairs when shapes have been deleted
		if (pairs[i].flags & (PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER | PxTriggerPairFlag::eREMOVED_SHAPE_OTHER))
			continue;

		if((pairs[i].otherActor == mSubmarineActor) && (pairs[i].triggerActor == gTreasureActor))
		{
			gTreasureFound = true;
		}
	}
#endif
}

void FDyPxSimulationEventCallback::onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count)
{

}

void FDyPxSimulationEventCallback::onWake(physx::PxActor** actors, physx::PxU32 count)
{

}

void FDyPxSimulationEventCallback::onSleep(physx::PxActor** actors, physx::PxU32 count)
{

}

void FDyPxSimulationEventCallback::onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count)
{

}

} /// ::dy namespace