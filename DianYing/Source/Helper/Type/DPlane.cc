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
#include <Dy/Helper/Type/DPlane.h>
#include <Dy/Helper/Math/Math.h>
#include <Math/Utility/XMath.h>

namespace dy
{

void DPlane::Normalize()
{
  const auto magnitude = this->GetNormalVector().GetLength();
  this->A /= magnitude;
  this->B /= magnitude;
  this->C /= magnitude;
  this->D /= magnitude;
}

DVec3 DPlane::GetNormalVector() const noexcept
{
  return DVec3{this->A, this->B, this->C};
}

DPlane::EStatus DPlane::CheckPointStatusOnPlane(const DVec3& iPoint) const noexcept
{
  const auto result = GetDistanceFrom(iPoint, false);
  if (math::IsNearlyZero(result, 0.0001f) == true) 
  { return DPlane::EStatus::OnPlane; }
  else if (result > 0.0f) 
  { return DPlane::EStatus::Front; }
  else 
  { return DPlane::EStatus::Behind; }
}

TF32 DPlane::GetDistanceFrom(const DVec3& iPoint, bool iIsAbs) const noexcept
{
  const auto result = iPoint.X * this->A + iPoint.Y * this->B * iPoint.Z * this->C + this->D;
  const auto parent = this->GetNormalVector().GetLength();
  if (result < 0 && iIsAbs == true)
  {
    return std::abs(result) / parent;
  }
  else
  {
    return result / parent;
  }
}

} /// ::dy namespace