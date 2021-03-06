#ifndef GUARD_DY_COMPONENT_INTENAL_CAMERA_DDYFRUSTUMTESTER_H
#define GUARD_DY_COMPONENT_INTENAL_CAMERA_DDYFRUSTUMTESTER_H
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

#include <array>
#include <Dy/Helper/Type/DPlane.h>
#include <Dy/Helper/Type/DMatrix4x4.h>

namespace dy
{

class FFrustumTester final
{
public:
  /// @brief Update frustum.
  void UpdateFrustum(const DMat4& mProjection, const DMat4& mView);

  /// @brief Check point is in frustum.
  MDY_NODISCARD bool IsPointInFrustum(const DVec3& mPoint) const noexcept;

  /// @brief Check virtual sphere that has point and radius is in frustum.
  MDY_NODISCARD bool IsSphereInFrustum(const DVec3& iPoint, TF32 iRadius) const noexcept;

private:
  /// @enum EDirection
  /// @brief Direction enum for helping readability.
  enum EDirection
  {
    DirRight  = 0, DirLeft   = 1, DirBottom = 2,
    DirTop    = 3, DirBack   = 4, DirFront  = 5
  };

  std::array<DPlane, 6> mFrustum = {};
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_INTENAL_CAMERA_DDYFRUSTUMTESTER_H
