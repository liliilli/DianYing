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
#include <Dy/Management/WorldManager.h>
#include <Dy/Management/LoggingManager.h>
#include <Dy/Management/MetaInfoManager.h>

#include <Dy/Element/Pawn.h>

namespace dy
{

EDySuccess MDyWorld::pfInitialize()
{
  MDY_LOG_INFO_D("{} | MDyWorld::pfInitialize().", "FunctionCall");
  return DY_SUCCESS;
}

EDySuccess MDyWorld::pfRelease()
{
  MDY_LOG_INFO_D("{} | MDyWorld::pfRelease().", "FunctionCall");
  if (this->mLevel)
  {
    this->mLevel->Release();
    this->mLevel = nullptr;
    this->Update(-1);
  }

  return DY_SUCCESS;
}

void MDyWorld::Update(float dt)
{
  // Garbage collect needless "FDyActor"s
  if (!this->mActorGc.empty())
  {
    this->mActorGc.clear();
  }

  // Remove activated pawn update list reversely.
  if (!this->mErasionPawnCandidateList.empty())
  {
    std::sort(MDY_BIND_BEGIN_END(this->mErasionPawnCandidateList), std::greater<TI32>());
    for (const auto& index : this->mErasionPawnCandidateList)
    { // Remove!
      this->mActivatedPawn.erase(this->mActivatedPawn.begin() + index);
    }
    // Clear!
    this->mErasionPawnCandidateList.clear();
  }

  // Travel next level
  if (!this->mNextLevelName.empty())
  {
    if (this->mLevel)
    { // Let present level do release sequence
      this->mLevel->Release();
    }

    auto& instance            = MDyMetaInfo::GetInstance();
    const auto* levelMetaInfo = instance.GetLevelMetaInformation(this->mNextLevelName);

    this->mLevel = std::make_unique<FDyLevel>();
    this->mLevel->Initialize(*levelMetaInfo);

    this->mPreviousLevelName  = this->mPresentLevelName;
    this->mPresentLevelName   = this->mNextLevelName;
    this->mNextLevelName      = MDY_NOT_INITILAIZED_STR;
  }

  // Scene update routine
  if (this->mLevel)
  {
    this->mLevel->Update(dt);
  }
}

void MDyWorld::UpdateObjects(float dt)
{
  if (this->mLevel)
  {
    for (auto& pawnPtr : this->mActivatedPawn)
    {
      if (pawnPtr == nullptr) continue;
      pawnPtr->ToString();
    }
  }
}

CDyCamera* MDyWorld::GetMainCameraPtr() const noexcept
{
  return this->mValidMainCameraPtr;
}

EDySuccess MDyWorld::OpenLevel(const std::string& levelName)
{
  if (MDyMetaInfo::GetInstance().GetLevelMetaInformation(levelName) == nullptr)
  {
    MDY_LOG_ERROR("{} | Failed to find and travel next level. Level name is not found. | Level name : {}", levelName);
    return DY_FAILURE;
  }

  this->mNextLevelName = levelName;
  return DY_SUCCESS;
}

void MDyWorld::__pfBindFocusCamera(CDyCamera* validCameraPtr)
{
  PHITOS_ASSERT(validCameraPtr != nullptr, "validCameraPtr must be valid, not nullptr.");
  this->mValidMainCameraPtr = validCameraPtr;
}

void MDyWorld::__pfUnbindCameraFocus()
{
  if (this->mValidMainCameraPtr)
  {
    this->mValidMainCameraPtr = nullptr;
    MDY_LOG_INFO_D("{} | MainCamera pointing unbinded.", "MDyWorld::__pfUnbindCameraFocus()");
  }
  else
  {
    MDY_LOG_WARNING_D("{} | Valid mainCamera pointer does not point anything.", "MDyWorld::__pfUnbindCameraFocus()");
  }
}

void MDyWorld::pfMoveActorToGc(NotNull<FDyActor*> actorRawPtr) noexcept
{
  this->mActorGc.emplace_back(std::unique_ptr<FDyActor>(actorRawPtr));
}

TI32 MDyWorld::pfEnrollActivePawn(const NotNull<FDyPawn*>& pawnRawPtr) noexcept
{
  this->mActivatedPawn.emplace_back(pawnRawPtr);
  return static_cast<TI32>(this->mActivatedPawn.size()) - 1;
}

void MDyWorld::pfUnenrollActivePawn(TI32 index) noexcept
{
  this->mActivatedPawn[index] = nullptr;
  this->mErasionPawnCandidateList.emplace_back(index);
}

} /// ::dy namespace