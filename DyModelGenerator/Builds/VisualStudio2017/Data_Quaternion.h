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

#include "Data_Matrix4.h"
#include <assimp/quaternion.h>
#include <glm/gtc/quaternion.hpp>

///
/// @class DDyQuaternion
/// @brief Quaternion class
///
class DDyQuaternion final {
public:
  DDyQuaternion() = default;
  DDyQuaternion(const DDyQuaternion& quaternion)            = default;
  DDyQuaternion& operator=(const DDyQuaternion& quaternion) = default;

  DDyQuaternion(const aiQuaternion& aiQuaternion);
  DDyQuaternion& operator=(const aiQuaternion& auQuaternion);

  /// @brief Get rotation matrix (4x4) from quaternion.
  DDyMatrix4x4 GetRotationMatrix4x4() const noexcept;

private:
  glm::quat mQuaternion;
};

