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

#include <Math/Type/Math/DMatrix4.h>
#include <Math/Type/Micellanous/EGraphics.h>
#include <Math/Type/Micellanous/EProjection.h>

namespace dy::math
{

/// @brief Create projection matrix following graphics and project value.
/// @tparam TType result type.
template <typename TType>
DMatrix4<TType> ProjectionMatrix(
  EGraphics graphics, EProjection project,
  TReal left, TReal right, TReal botton, TReal top, TReal near, TReal far);

/// @brief Create projection matrix following graphics and project value.
/// @tparam TType result type.
template <typename TType>
DMatrix4<TType> ProjectionMatrix(
  EGraphics graphics, EProjection project,
  TReal fovY, TReal width, TReal height, TReal near, TReal far);

template <typename TType>
DMatrix4<TType> LookAt(
  const DVector3<TType>& position, 
  const DVector3<TType>& lookPosition,
  const DVector3<TType>& unitY);

} /// ::dy::math namespace
#include <Math/Utility/Inline/XGraphicsMath.inl>