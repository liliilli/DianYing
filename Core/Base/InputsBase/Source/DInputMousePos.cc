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
#include <DInputMousePos.h>

namespace std
{

/// @brief Only used in this translation unit.
std::pair<int, int> 
operator-(const std::pair<int, int>& lhs, const std::pair<int, int>& rhs) noexcept
{
  return {lhs.first - rhs.first, lhs.second - rhs.second};
}

} /// ::std namespace 

namespace dy::base
{

ELowMousePosState DInputMousePos::GetFeatureState() const noexcept
{
  return this->mState;
}

void DInputMousePos::UpdatePosition(int x, int y)
{
  switch (this->mState)
  {
  case ELowMousePosState::Normal: 
  {
    if (this->mIsMovedFirst == false)
    {
      this->mPosX = this->mPrePosX = x; this->mPosY = this->mPrePosY = y;
      this->mIsMovedFirst = true;
    }
    else
    {
      // Move old position to prepos.
      this->mPrePosX = this->mPosX; this->mPrePosY = this->mPosY;
      // Move new position to pos.
      this->mPosX = x; this->mPosY = y;
    }
  } break;
  case ELowMousePosState::Unlimited: 
  {
    if (this->mIsMovedFirst == false)
    {
      this->mPosX = this->mPrePosX = x; this->mPosY = this->mPrePosY = y;
      this->mIsMovedFirst = true;
    }
    else
    {
      // Move old position to prepos.
      this->mPrePosX = this->mPosX; this->mPrePosY = this->mPosY;
      // Move new position to pos.
      this->mPosX = x ;
      this->mPosY = y;
    }
  } break;
  default: /* Do nothing */ break;
  }
}

void DInputMousePos::SetFeatureState(ELowMousePosState newState) noexcept
{
  // Apply enum value into member variable.
  this->mState = newState; 

  // Reset values.
  switch (this->mState)
  {
  case ELowMousePosState::Normal: 
  case ELowMousePosState::Unlimited: 
  {
    this->mPosX = this->mPosY = this->mPrePosX = this->mPrePosY = 0;
    this->mIsMovedFirst = false;
  } break;
  case ELowMousePosState::Off: 
  {
    this->mPosX = this->mPosY = this->mPrePosX = this->mPrePosY = -1;
    this->mIsMovedFirst = false;
  } break;
  }
}

std::pair<int, int> DInputMousePos::GetPresentPosition() const noexcept
{
  return {this->mPosX, this->mPosY};
}

std::pair<int, int> DInputMousePos::GetPreviousPosition() const noexcept
{
  return {this->mPrePosX, this->mPrePosY};
}

std::pair<int, int> DInputMousePos::GetMovementPosition() const noexcept
{
  return this->GetPresentPosition() - this->GetPreviousPosition();
}

bool DInputMousePos::HasInitiallyMoved() const noexcept
{
  return this->mIsMovedFirst;
}

} /// ::dy::base namespace