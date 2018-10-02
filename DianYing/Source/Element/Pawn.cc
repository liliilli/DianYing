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

/// Header file
#include <Dy/Element/Pawn.h>
#include <Dy/Element/Descriptor/LevelDescriptor.h>
#include <Dy/Management/WorldManager.h>

namespace dy
{

EDySuccess FDyPawn::Initialize(const DDyObjectInformation& desc)
{
  this->mActorType = desc.mType;
  this->pSetObjectName(desc.mName);
  this->SetWorldSpacePosition(desc.mTransform.mPosition);
  this->SetWorldEulerAngle(desc.mTransform.mRotation);
  this->SetWorldScale(desc.mTransform.mScale);

  const auto& scriptInfo = std::any_cast<const DDyFdyPawnDependencyInformation&>(desc.mDependencyInfo);
  MDY_LOG_CRITICAL("Script path : {}", scriptInfo.mScriptPath);
  return DY_SUCCESS;

  //!
  //! PHYSX TEST ROUTINE
  //!
#ifdef true

#endif
}

EDySuccess FDyPawn::Release()
{
  MDY_LOG_CRITICAL("FDyPawn::Release, {}", this->GetActorName());

  // If pawn is activated, request to MDyWorld to unenroll mine.
  if (this->mActivatedUpdateListId != MDY_INITIALIZE_DEFINT)
  {
    MDyWorld::GetInstance().pfUnenrollActivePawn(this->mActivatedUpdateListId);
  }
  return DY_SUCCESS;
}

void FDyPawn::Initiate()
{
  MDY_LOG_WARNING("FDyPawn::Initiate() | {}", this->GetActorName());
}

void FDyPawn::Start()
{
  MDY_LOG_WARNING("FDyPawn::Start() | {}", this->GetActorName());
}

void FDyPawn::Update(float dt)
{
  //MDY_LOG_WARNING("FDyPawn::Update() | {} : {}", this->GetActorName(), dt);
}

void FDyPawn::Destroy()
{
  MDY_LOG_WARNING("FDyPawn::Destroy() | {}", this->GetActorName());
}

void FDyPawn::OnEnabled()
{
  MDY_LOG_WARNING("FDyPawn::OnEnabled() | {}", this->GetActorName());
}

void FDyPawn::OnDisabled()
{
  MDY_LOG_WARNING("FDyPawn::OnDisabled() | {}", this->GetActorName());
}

void FDyPawn::pfSetListIndex(TI32 index) noexcept
{
  this->mActivatedUpdateListId = index;
}

} /// ::dy namespace