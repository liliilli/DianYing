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
#include <Dy/Element/Canvas/UiObject.h>
#include <Dy/Meta/Type/EDyWidgetTypes.h>

namespace dy
{

void FDyUiObject::SetWidgetCentralPosition(_MIN_ const DDyVector2& position) noexcept
{
  this->mCentralPosition = position;
}

void FDyUiObject::SetWidgetFrameSize(_MIN_ const DDyVectorInt2& size) noexcept
{
  if (size.X > 0 && size.Y > 0) { this->mWidgetSize = size; }
}

DDyVectorInt2 FDyUiObject::GetFrameSize() const noexcept
{
  return this->mWidgetSize;
}

DDyVector2 FDyUiObject::GetWidgetPosition(const EDyOrigin& origin) const noexcept
{
  switch (origin)
  {
  case EDyOrigin::Left_Bottom:  return this->mCentralPosition - (this->mWidgetSize / 2);
  case EDyOrigin::Left_Center:  return this->mCentralPosition - DDyVectorInt2{this->mWidgetSize.X >> 1, 0};
  case EDyOrigin::Left_Top:     return this->mCentralPosition + DDyVectorInt2{-(this->mWidgetSize.X >> 1), this->mWidgetSize.Y >> 1};
  case EDyOrigin::Center_Bottom:return this->mCentralPosition - DDyVectorInt2{0, this->mWidgetSize.Y >> 1};
  case EDyOrigin::Center_Center:return this->mCentralPosition;
  case EDyOrigin::Center_Top:   return this->mCentralPosition + DDyVectorInt2{0, this->mWidgetSize.Y >> 1};
  case EDyOrigin::Right_Bottom: return this->mCentralPosition + DDyVectorInt2{this->mWidgetSize.X >> 1, -(this->mWidgetSize.Y >> 1)};
  case EDyOrigin::Right_Center: return this->mCentralPosition + DDyVectorInt2{this->mWidgetSize.X >> 1, 0};
  case EDyOrigin::Right_Top:    return this->mCentralPosition + (this->mWidgetSize / 2);
  default: MDY_UNEXPECTED_BRANCH(); break;
  }

  MDY_UNEXPECTED_BRANCH();
  return {};
}

} /// ::dy namespace