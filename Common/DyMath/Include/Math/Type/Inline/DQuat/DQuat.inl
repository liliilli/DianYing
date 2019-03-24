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
DQuat<TType>::DQuat(const DVector3<TType>& eulerAngle, bool isDegree)
{
  const auto angle = isDegree == true 
    ? eulerAngle * kToRadian<TType> * TType(0.5) 
    : eulerAngle * TType(0.5);

  const auto cosAngle = Cos(angle);
  const auto sinAngle = Sin(angle);

  
}

template <typename TType>
DQuat<TType>::DQuat(TValueType x, TValueType y, TValueType z, TValueType w)
{

}

} /// ::dy::math namespace