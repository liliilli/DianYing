#include <precompiled.h>
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
#include <Dy/Management/Type/Input/DButtonInputItem.h>
#include <Dy/Management/MLog.h>

#include <Dy/Include/GlInclude.h>

//!
//! Local translation unit functions.
//!

namespace
{

dy::EInputButtonStatus GetStatusFromGLFWEnum(_MIN_ int iLowLevelStatus) noexcept
{
  switch (iLowLevelStatus)
  {
  case GLFW_PRESS:    return dy::EInputButtonStatus::Pressed;
  case GLFW_RELEASE:  return dy::EInputButtonStatus::Released;
  case GLFW_REPEAT:   return dy::EInputButtonStatus::Repeated;
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(dy::EInputButtonStatus::Released);
  }
};

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy
{

void DButtonInputItem::Update(int iLowLevelStatus) noexcept
{
  using TEnum = decltype(this->mStatus);
  const auto inputStatus = GetStatusFromGLFWEnum(iLowLevelStatus);

  switch (this->mStatus)
  {
  case TEnum::Released:
    switch (inputStatus)
    {
    case TEnum::Pressed: this->mStatus = TEnum::Pressed; break;
    default: break;
    }
    break;
  case TEnum::Pressed: 
    switch (inputStatus)
    {
    case TEnum::Pressed:  
    case TEnum::Repeated: this->mStatus = TEnum::Repeated; break;
    case TEnum::Released: this->mStatus = TEnum::Released; break;
    }
    break;
  case TEnum::Repeated:
  {
    switch (inputStatus)
    {
    case TEnum::Released: this->mStatus = TEnum::Released; break;
    default: break;
    }
  } break;
  }
}

EInputButtonStatus DButtonInputItem::Get() const noexcept
{
  return this->mStatus;
}

} /// ::dy namespace