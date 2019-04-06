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
#include <Type/XTypes.h>

namespace dy
{

using DVec2 = math::DVector2<TReal>;
using DVec3 = math::DVector3<TReal>;
using DVec4 = math::DVector4<TReal>;

using DVecI2 = math::DVector2<TI32>;
using DVecI3 = math::DVector3<TI32>;
using DVecI4 = math::DVector4<TI32>;

/// @struct FVec
/// @brief Factory static functions container type.
struct FVec final
{

};

} /// ::dy namesapce
