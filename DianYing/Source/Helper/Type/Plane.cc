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
#include <Dy/Helper/Type/Plane.h>

namespace dy
{

void DDyPlane::Normalize()
{
  const auto magnitude = this->GetNormalVector().GetLength();
  this->A /= magnitude;
  this->B /= magnitude;
  this->C /= magnitude;
  this->D /= magnitude;
}

DDyVector3 DDyPlane::GetNormalVector() const noexcept
{
  return DDyVector3{this->A, this->B, this->C};
}

DDyPlane::EStatus DDyPlane::CheckPointStatusOnPlane(const DDyVector3& iPoint) const noexcept
{
  const auto result = GetDistanceFrom(iPoint);
  if (math::IsNearlyZero(result, 0.001f) == true) 
  { return DDyPlane::EStatus::OnPlane; }
  else if (result > 0.0f) 
  { return DDyPlane::EStatus::Front; }
  else 
  { return DDyPlane::EStatus::Behind; }
}

TF32 DDyPlane::GetDistanceFrom(const DDyVector3& iPoint, bool iIsAbs) const noexcept
{
  const auto result = iPoint.X * this->A + iPoint.Y * this->B * iPoint.Z * this->C + this->D;
  if (result < 0 && iIsAbs == true)
  {
    return std::abs(result);
  }
  else
  {
    return result;
  }
}

} /// ::dy namespace