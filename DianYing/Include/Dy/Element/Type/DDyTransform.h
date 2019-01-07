#ifndef GUARD_DY_ELEMENT_TYPE_DDYTRANSFORM_H
#define GUARD_DY_ELEMENT_TYPE_DDYTRANSFORM_H
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

#include <Dy/Helper/Type/Vector3.h>

namespace dy
{

/// @struct DDyTransform
/// @brief Primal transform list.
struct DDyTransform
{
  DDyVector3 mLocalRelPosition  = {0, 0, 0};
  DDyVector3 mWorldRelPosition  = {0, 0, 0};

  DDyVector3 mLocalEulerAngle   = {0, 0, 0};
  DDyVector3 mWorldEulerAngle   = {0, 0, 0};

  DDyVector3 mLocalScale        = {1, 1, 1};
  DDyVector3 mWorldScale        = {1, 1, 1};
};

} /// ::dy namespace

#endif /// GUARD_DY_ELEMENT_TYPE_DDYTRANSFORM_H