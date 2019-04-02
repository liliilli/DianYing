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

#include <Math/Type/Math/DMatrix2.h>
#include <Math/Type/Math/DMatrix3.h>
#include <Math/Type/Math/DMatrix4.h>
#include <Math/Type/Math/DQuat.h>
#include <Type/XTypes.h>

namespace dy
{

using DMat2 = math::DMatrix2<TReal>;
using DMat3 = math::DMatrix3<TReal>;
using DMat4 = math::DMatrix4<TReal>;

using DQuat = math::DQuaternion<TReal>;

} /// ::dy namesapce
