#include <precompiled.h>
///
/// MIT License
/// Copyright (c) 2018 Jongmin Yun
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

namespace dy
{

DDyAxisBindingInformation::DDyAxisBindingInformation(_MIN_ const DDySettingInput::DAxis& axisInfo)
{
  this->mIsRepeatKey        = axisInfo.mIsRepeatable;
  this->mAxisSpecifierName  = axisInfo.mSpecifierName;
  this->mGravity            = axisInfo.mGravity;

  for (const auto& keyEnumValue : axisInfo.mPositive)
  {
    this->mPositiveButtonId.emplace_back(static_cast<std::underlying_type_t<EDyKeyboard>>(keyEnumValue));
  }
  for (const auto& keyEnumValue : axisInfo.mNegative)
  {
    this->mNegativeButtonId.emplace_back(static_cast<std::underlying_type_t<EDyKeyboard>>(keyEnumValue));
  }

  MDY_ASSERT(this->mGravity > 0, "Gravity must be bigger than 0.");
}

} /// ::dy namespace