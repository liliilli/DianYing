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

#include <Math/Utility/XMath.h>

namespace dy::math
{

template <typename TType>
DVector2<TType> Sin(const DVector2<TType>& vector, bool isDegree)
{
  if (isDegree == true)
  {
    return 
    {
      std::sin(vector.X * kToRadian<TType>), 
      std::sin(vector.Y * kToRadian<TType>)
    };
  }
  else
  {
    return {std::sin(vector.X), std::sin(vector.Y)};
  }
}
  
template <typename TType>
DVector3<TType> Sin(const DVector3<TType>& vector, bool isDegree)
{
  if (isDegree == true)
  {
    return 
    {
      std::sin(vector.X * kToRadian<TType>), 
      std::sin(vector.Y * kToRadian<TType>),
      std::sin(vector.Z * kToRadian<TType>)
    };
  }
  else
  {
    return 
    {
      std::sin(vector.X), 
      std::sin(vector.Y), 
      std::sin(vector.Z)
    };
  }
}
  
template <typename TType>
DVector4<TType> Sin(const DVector4<TType>& vector, bool isDegree)
{
  if (isDegree == true)
  {
    return 
    {
      std::sin(vector.X * kToRadian<TType>), 
      std::sin(vector.Y * kToRadian<TType>),
      std::sin(vector.Z * kToRadian<TType>),
      std::sin(vector.W * kToRadian<TType>)
    };
  }
  else
  {
    return 
    {
      std::sin(vector.X), 
      std::sin(vector.Y), 
      std::sin(vector.Z),
      std::sin(vector.W)
    };
  }
}
  
template <typename TType>
DVector2<TType> Cos(const DVector2<TType>& vector, bool isDegree)
{
  if (isDegree == true)
  {
    return 
    {
      std::cos(vector.X * kToRadian<TType>), 
      std::cos(vector.Y * kToRadian<TType>)
    };
  }
  else
  {
    return {std::cos(vector.X), std::cos(vector.Y)};
  }
}
  
template <typename TType>
DVector3<TType> Cos(const DVector3<TType>& vector, bool isDegree)
{
  if (isDegree == true)
  {
    return 
    {
      std::cos(vector.X * kToRadian<TType>), 
      std::cos(vector.Y * kToRadian<TType>),
      std::cos(vector.Z * kToRadian<TType>)
    };
  }
  else
  {
    return 
    {
      std::cos(vector.X), 
      std::cos(vector.Y), 
      std::cos(vector.Z)
    };
  }
}
  
template <typename TType>
DVector4<TType> Cos(const DVector4<TType>& vector, bool isDegree)
{
  if (isDegree == true)
  {
    return 
    {
      std::cos(vector.X * kToRadian<TType>), 
      std::cos(vector.Y * kToRadian<TType>),
      std::cos(vector.Z * kToRadian<TType>),
      std::cos(vector.W * kToRadian<TType>)
    };
  }
  else
  {
    return 
    {
      std::cos(vector.X), 
      std::cos(vector.Y), 
      std::cos(vector.Z),
      std::cos(vector.W)
    };
  }
}

template <typename TType>
DVector2<TType> Tan(const DVector2<TType>& vector, bool isDegree)
{
  if (isDegree == true)
  {
    return 
    {
      std::tan(vector.X * kToRadian<TType>), 
      std::tan(vector.Y * kToRadian<TType>)
    };
  }
  else
  {
    return {std::tan(vector.X), std::tan(vector.Y)};
  }
}
  
template <typename TType>
DVector3<TType> Tan(const DVector3<TType>& vector, bool isDegree)
{
  if (isDegree == true)
  {
    return 
    {
      std::tan(vector.X * kToRadian<TType>), 
      std::tan(vector.Y * kToRadian<TType>),
      std::tan(vector.Z * kToRadian<TType>)
    };
  }
  else
  {
    return 
    {
      std::tan(vector.X), 
      std::tan(vector.Y), 
      std::tan(vector.Z)
    };
  }
}
  
template <typename TType>
DVector4<TType> Tan(const DVector4<TType>& vector, bool isDegree)
{
  if (isDegree == true)
  {
    return 
    {
      std::tan(vector.X * kToRadian<TType>), 
      std::tan(vector.Y * kToRadian<TType>),
      std::tan(vector.Z * kToRadian<TType>),
      std::tan(vector.W * kToRadian<TType>)
    };
  }
  else
  {
    return 
    {
      std::tan(vector.X), 
      std::tan(vector.Y), 
      std::tan(vector.Z),
      std::tan(vector.W)
    };
  }
}

} /// ::dy::math namespace