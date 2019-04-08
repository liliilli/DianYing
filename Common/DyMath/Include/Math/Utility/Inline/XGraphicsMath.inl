#pragma once
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

#include <Math/Utility/XGraphicsMath.h>
#include <Math/Utility/XLinearMath.h>

#ifdef near
#undef near
#endif
#ifdef far
#undef far
#endif

namespace dy::math::detail
{

/// @brief Get orthographic projection of OpenGL. \n
/// [ 2/(r-l)    0       0    -(r+l)/(r-l) ] \n
/// [   0     2/(t-b)    0    -(t+b)/(t-b) ] \n
/// [   0        0   -2/(f-n) -(f+n)/(f-n) ] \n
/// [   0        0       0          1      ]
template <typename TType>
DMatrix4<TType> OpenGlOrtho(TReal left, TReal right, TReal bottom, TReal top, TReal near, TReal far)
{
  DMatrix4<TType> result{};

  result[0][0] = static_cast<TType>(2) / (right - left);
  result[1][1] = static_cast<TType>(2) / (top - bottom);
  result[2][2] = - static_cast<TType>(2) / (far - near);
  result[3][0] = - (right + left) / (right - left);
  result[3][1] = - (top + bottom) / (top - bottom);
  result[3][2] = - (far + near) / (far - near);
  result[3][3] = 1;

  return result;
}

template <typename TType>
DMatrix4<TType> OpenGlOrtho(TReal fovY, TReal width, TReal height, TReal near, TReal far)
{
  return {};
}

template <typename TType>
DMatrix4<TType> OpenGlPersp(TReal left, TReal right, TReal bottom, TReal top, TReal near, TReal far)
{
  return {};
}

template <typename TType>
DMat4 OpenGlPersp(TReal fovY, TReal width, TReal height, TReal near, TReal far)
{
  const TType halfFovyTan = std::tan(fovY / TType(2));
  const TType aspect = width / height;
  DMatrix4<TType> result{};

  result[0][0] = TType(1) / (aspect * halfFovyTan);
  result[1][1] = TType(1) / (halfFovyTan);
  result[2][2] = -(far + near) / (far - near);
  result[2][3] = -TType(1);
  result[3][2] = -(TType(2) * far * near) / (far - near);
  return result;
}

} /// ::dy::math::detail namespace

namespace dy::math
{

template <typename TType>
DMatrix4<TType> ProjectionMatrix(
  EGraphics graphics, EProjection project,
  TReal left, TReal right, TReal bottom, TReal top, TReal near, TReal far)
{
  switch (graphics)
  {
  case EGraphics::OpenGL: 
  {
    if (project == EProjection::Orthogonal) 
    { 
      return detail::OpenGlOrtho<TType>(left, right, bottom, top, near, far);
    }
    else
    {
      return detail::OpenGlPersp<TType>(left, right, bottom, top, near, far);
    }
  } break;
  }
  return {};
}

template <typename TType>
DMatrix4<TType> ProjectionMatrix(
  EGraphics graphics, EProjection project, 
  TReal fovY, TReal width, TReal height, TReal near, TReal far)
{
  switch (graphics)
  {
  case EGraphics::OpenGL: 
  {
    if (project == EProjection::Orthogonal) 
    { 
      return detail::OpenGlOrtho<TType>(fovY, width, height, near, far);
    }
    else
    {
      return detail::OpenGlPersp<TType>(fovY, width, height, near, far);
    }
  } break;
  }
  return {};
}

template <typename TType>
DMatrix4<TType> LookAt(
  const DVector3<TType>& position, 
  const DVector3<TType>& lookPosition,
  const DVector3<TType>& unitY)
{
  const DVector3<TType> unitFront = (lookPosition - position).Normalize();
  const DVector3<TType> unitSide  = Cross(unitFront, unitY).Normalize();
  const DVector3<TType> unitUp    = Cross(unitSide, unitFront);

  //  sx  sy  sz -dot(s,p)
  //  ux  uy  uz -dot(u,p)
  // -fx -fy -fz -dot(f,p)
  //   0   0   0     1
  DMatrix4<TType> result{};
  result[0][0] =  unitSide.X;
  result[1][0] =  unitSide.Y;
  result[2][0] =  unitSide.Z;
  result[0][1] =  unitUp.X;
  result[1][1] =  unitUp.Y;
  result[2][1] =  unitUp.Z;
  result[0][2] = -unitFront.X;
  result[1][2] = -unitFront.Y;
  result[2][2] = -unitFront.Z;
  result[3][0] = -Dot(unitSide, position);
  result[3][1] = -Dot(unitUp, position);
  result[3][2] =  Dot(unitFront, position);  
  result[3][3] =  1;

  return result;
}

} /// ::dy::math namespace