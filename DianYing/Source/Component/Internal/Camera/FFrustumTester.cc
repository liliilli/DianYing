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
#include <Dy/Component/Internal/Camera/FFrustumTester.h>
#include <Dy/Helper/Type/DMatrix4x4.h>
#include <Dy/Management/MLog.h>

namespace dy
{

void FFrustumTester::UpdateFrustum(const DMat4& mProjection, const DMat4& mView)
{
  const auto clipMatrix = mProjection * mView;

  // Setup right plane and normalize.
  mFrustum[DirRight].A = clipMatrix[0][3] - clipMatrix[0][0];
  mFrustum[DirRight].B = clipMatrix[1][3] - clipMatrix[1][0];
  mFrustum[DirRight].C = clipMatrix[2][3] - clipMatrix[2][0];
  mFrustum[DirRight].D = clipMatrix[3][3] - clipMatrix[3][0];
  mFrustum[DirRight].Normalize();

  // Setup left plane and normalize.
  mFrustum[DirLeft].A = clipMatrix[0][3] + clipMatrix[0][0];
  mFrustum[DirLeft].B = clipMatrix[1][3] + clipMatrix[1][0];
  mFrustum[DirLeft].C = clipMatrix[2][3] + clipMatrix[2][0];
  mFrustum[DirLeft].D = clipMatrix[3][3] + clipMatrix[3][0];
  mFrustum[DirLeft].Normalize();
 
  // Setup top plane and normalize.
  mFrustum[DirTop].A = clipMatrix[0][3] - clipMatrix[0][1];
  mFrustum[DirTop].B = clipMatrix[1][3] - clipMatrix[1][1];
  mFrustum[DirTop].C = clipMatrix[2][3] - clipMatrix[2][1];
  mFrustum[DirTop].D = clipMatrix[3][3] - clipMatrix[3][1];
  mFrustum[DirTop].Normalize();
  
  // Setup bottom plane and normalize.
  mFrustum[DirBottom].A = clipMatrix[0][3] + clipMatrix[0][1];
  mFrustum[DirBottom].B = clipMatrix[1][3] + clipMatrix[1][1];
  mFrustum[DirBottom].C = clipMatrix[2][3] + clipMatrix[2][1];
  mFrustum[DirBottom].D = clipMatrix[3][3] + clipMatrix[3][1];
  mFrustum[DirBottom].Normalize();
 
  // Setup back plane and normalize.
  mFrustum[DirBack].A = clipMatrix[0][3] - clipMatrix[0][2];
  mFrustum[DirBack].B = clipMatrix[1][3] - clipMatrix[1][2];
  mFrustum[DirBack].C = clipMatrix[2][3] - clipMatrix[2][2];
  mFrustum[DirBack].D = clipMatrix[3][3] - clipMatrix[3][2];
  mFrustum[DirBack].Normalize();
   
  // Setup front plane and normalize.
  mFrustum[DirFront].A = clipMatrix[0][3] + clipMatrix[0][2];
  mFrustum[DirFront].B = clipMatrix[1][3] + clipMatrix[1][2];
  mFrustum[DirFront].C = clipMatrix[2][3] + clipMatrix[2][2];
  mFrustum[DirFront].D = clipMatrix[3][3] + clipMatrix[3][2];
  mFrustum[DirFront].Normalize();
}

bool FFrustumTester::IsPointInFrustum(const DVec3& mPoint) const noexcept
{
  for (auto i = 0; i < 6; ++i)
  { 
    // Calculate the plane equation and check if the point is behind a side of the frustum.
    if (mFrustum[i].CheckPointStatusOnPlane(mPoint) == DPlane::EStatus::Behind) 
    { 
      return false; 
    }
  }
  return true;
}

bool FFrustumTester::IsSphereInFrustum(const DVec3& iPoint, TF32 iRadius) const noexcept
{
  if (iRadius < 0)
  {
    DyPushLogDebugError(
      "Given radius is negative value {}, Failed to calculate frustum testing.",
      iRadius);
    return false;
  }

  for (auto i = 0; i < 6; ++i)
  {
    // Calculate the plane equation and check if the point is behind a side of the frustum.
    if (mFrustum[i].CheckPointStatusOnPlane(iPoint) == DPlane::EStatus::Behind) 
    { 
      const auto distance = mFrustum[i].GetDistanceFrom(iPoint, true);
      if (distance > iRadius) 
      { 
        return false; 
      }
    }    
  }
  return true;
}

} /// ::dy namespace