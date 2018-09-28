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
#include <Dy/Management/SceneManager.h>
#include <Dy/Management/LoggingManager.h>
#include <Dy/Management/ExternalResouceInfoManager.h>

namespace dy
{

EDySuccess MDyScene::pfInitialize()
{
  MDY_LOG_INFO_D("{} | MDyScene::pfInitialize().", "FunctionCall");
  return DY_SUCCESS;
}

EDySuccess MDyScene::pfRelease()
{
  MDY_LOG_INFO_D("{} | MDyScene::pfRelease().", "FunctionCall");
  return DY_SUCCESS;
}

void MDyScene::Update(float dt)
{
  // Travel next level
  if (!this->mNextLevelName.empty())
  {
    if (this->mLevel)
    { // Let present level do release sequence
      this->mLevel->Release();
    }

    auto& instance            = MDyExtRscInfo::GetInstance();
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

void MDyScene::UpdateObjects(float dt)
{
  if (this->mLevel)
  {
    PHITOS_NOT_IMPLEMENTED_ASSERT();
  }
}

CDyCamera* MDyScene::GetMainCameraPtr() const noexcept
{
  return this->mValidMainCameraPtr;
}

EDySuccess MDyScene::OpenLevel(const std::string& levelName)
{
  if (MDyExtRscInfo::GetInstance().GetLevelMetaInformation(levelName) == nullptr)
  {
    MDY_LOG_ERROR("{} | Failed to find and travel next level. Level name is not found. | Level name : {}", levelName);
    return DY_FAILURE;
  }

  this->mNextLevelName = levelName;
  return DY_SUCCESS;
}

void MDyScene::__pfBindFocusCamera(CDyCamera* validCameraPtr)
{
  PHITOS_ASSERT(validCameraPtr != nullptr, "validCameraPtr must be valid, not nullptr.");
  this->mValidMainCameraPtr = validCameraPtr;
}

void MDyScene::__pfUnbindCameraFocus()
{
  if (this->mValidMainCameraPtr)
  {
    this->mValidMainCameraPtr = nullptr;
    MDY_LOG_INFO_D("{} | MainCamera pointing unbinded.", "MDyScene::__pfUnbindCameraFocus()");
  }
  else
  {
    MDY_LOG_WARNING_D("{} | Valid mainCamera pointer does not point anything.", "MDyScene::__pfUnbindCameraFocus()");
  }
}

} /// ::dy namespace