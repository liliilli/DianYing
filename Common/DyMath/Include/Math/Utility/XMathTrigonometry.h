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

#include <Math/Type/Math/DVector2.h>
#include <Math/Type/Math/DVector3.h>
#include <Math/Type/Math/DVector4.h>

namespace dy::math
{
  
template <typename TType>
DVector2<TType> Sin(const DVector2<TType>& vector);
  
template <typename TType>
DVector3<TType> Sin(const DVector3<TType>& vector);
  
template <typename TType>
DVector4<TType> Sin(const DVector4<TType>& vector);
  
template <typename TType>
DVector3<TType> Cos(const DVector2<TType>& vector);
  
template <typename TType>
DVector4<TType> Cos(const DVector3<TType>& vector);
  
template <typename TType>
DVector4<TType> Cos(const DVector4<TType>& vector);

} /// ::dy::math namespace