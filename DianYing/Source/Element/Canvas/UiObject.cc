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
  return DyGetPositionWithOrigin(this->mCentralPosition, this->mWidgetSize, origin);
}

} /// ::dy namespace