#ifndef GUARD_DY_HELPER_TYPE_PLANE_H
#define GUARD_DY_HELPER_TYPE_PLANE_H
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

#include <Dy/Helper/Type/DVector3.h>

namespace dy
{

/// @struct DPlane
/// @brief Plane type.
struct DPlane final
{
  enum EStatus
  {
    Front,
    Behind,
    OnPlane
  };

  TF32 A = 0;
  TF32 B = 0;
  TF32 C = 0;
  TF32 D = 0;

  /// @brief Normalize plane.
  void Normalize();

  /// @brief Get normal vector.
  MDY_NODISCARD DVector3 GetNormalVector() const noexcept;

  /// @brief Check given point is on plane, on front (pos), on behind (neg).
  MDY_NODISCARD EStatus CheckPointStatusOnPlane(const DVector3& iPoint) const noexcept;
  
  /// @brief Get given point's distance from plane.
  /// If second parameter is false, return value without absolute value.
  MDY_NODISCARD TF32 GetDistanceFrom(const DVector3& iPoint, bool iIsAbs = true) const noexcept;
};

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_TYPE_PLANE_H
