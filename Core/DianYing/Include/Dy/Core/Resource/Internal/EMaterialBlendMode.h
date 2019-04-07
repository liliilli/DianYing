#ifndef GUARD_DY_CORE_COMPONENT_INTERNAL_MATERIAL_TYPE_H
#define GUARD_DY_CORE_COMPONENT_INTERNAL_MATERIAL_TYPE_H
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

//!
//! Implementation
//!

namespace dy
{

/// @enum EMaterialBlendMode
/// @brief Specify material's blend mode. The way of rendering will be changed by this value.
enum class EMaterialBlendMode
{
  // Material will be forward to deferred rendering (pass 2)
  Opaque,
  // Material will be forward to forwarding rendering afterward deferred rendering (pass 3)
  TranslucentOIT,
  // I dont know but will be used later. (pass x)
  Custom,
};

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const EMaterialBlendMode& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ EMaterialBlendMode& p);

} /// ::dy namespace

#endif /// GUARD_DY_CORE_COMPONENT_INTERNAL_MATERIAL_TYPE_H