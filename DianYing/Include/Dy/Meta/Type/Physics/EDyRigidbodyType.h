#ifndef GUARD_DY_META_TYPE_PHYSICS_EDyRigidbodyType_H
#define GUARD_DY_META_TYPE_PHYSICS_EDyRigidbodyType_H
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

#include <nlohmann/json_fwd.hpp>

namespace dy
{

/// @enum EDyRigidbodyType
/// @brief Rigidbody type.
enum class EDyRigidbodyType
{
  Static,     // Static type that does not reflect gravity and fixed.
  Kinematic,  // Kinematic type does not reflect gravity but reflect collision effects.
  Dynamic     // Dynamic type can reflect gravity and reflect collision effects.
};

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const EDyRigidbodyType& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ EDyRigidbodyType& p);

} /// ::dy namespace

#endif /// GUARD_DY_META_TYPE_PHYSICS_EDyRigidbodyType_H