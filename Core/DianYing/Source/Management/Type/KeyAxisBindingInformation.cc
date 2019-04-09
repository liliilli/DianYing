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
#include <Dy/Management/Type/KeyAxisBindingInformation.h>
#include <Dy/Helper/System/Assertion.h>

namespace dy
{

DDyAxisBindingInformation::DDyAxisBindingInformation(_MIN_ const DDySettingInput::DAxis& axisInfo)
{
  this->mAxisSpecifierName  = axisInfo.mSpecifierName;
  this->mGravity            = axisInfo.mGravity;
  this->mPositiveButtonId   = axisInfo.mPositive;
  this->mNegativeButtonId   = axisInfo.mNegative;

  MDY_ASSERT_MSG(this->mGravity > 0, "Gravity must be bigger than 0.");
}

} /// ::dy namespace