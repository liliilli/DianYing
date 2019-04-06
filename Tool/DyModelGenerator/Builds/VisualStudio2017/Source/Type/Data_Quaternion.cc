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
#include "../../Include/Type/Data_Quaternion.h"
#include "juce_core/system/juce_PlatformDefs.h"

DDyQuaternion::DDyQuaternion(const aiQuaternion& aiQuaternion) :
  mQuaternion{aiQuaternion.w, aiQuaternion.x, aiQuaternion.y, aiQuaternion.z}
{ }

DDyQuaternion& DDyQuaternion::operator=(const aiQuaternion& aiQuatenrion)
{
  this->mQuaternion = glm::quat{aiQuatenrion.w, aiQuatenrion.x, aiQuatenrion.y, aiQuatenrion.z};
  return *this;
}

DDyMatrix4x4 DDyQuaternion::GetRotationMatrix4x4() const noexcept {

  const auto glmMatrix = glm::mat4_cast(this->mQuaternion);
  DDyMatrix4x4 mMatrixValue;
  mMatrixValue[0] = glmMatrix[0];
  mMatrixValue[1] = glmMatrix[1];
  mMatrixValue[2] = glmMatrix[2];
  mMatrixValue[3] = glmMatrix[3];

  return mMatrixValue;
}
