#ifndef GUARD_DY_TYPE_PHYSICS_EDyColliderType_H
#define GUARD_DY_TYPE_PHYSICS_EDyColliderType_H
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

#include <Dy/Meta/Information/Physics/DColliderMetaInfo.h>

namespace dy
{

/// @enum EDyColliderType
/// @brief Collider type.
enum class EDyColliderType
{
  Sphere,
  Capsule,
  Box,

  NoneError
};

template <EDyColliderType TType> 
struct TColliderBindingType
{ 
  using Type = void; 
  TColliderBindingType() { static_assert(std::is_same_v<Type, void>, "Not supported type."); }
};

template <> struct TColliderBindingType<EDyColliderType::Sphere> { using Type = PDySphereColliderMetaInfo; };
template <> struct TColliderBindingType<EDyColliderType::Capsule> { using Type = PDyCapsulesColliderMetaInfo; };
template <> struct TColliderBindingType<EDyColliderType::Box> { using Type = PDyBoxColliderMetaInfo; };

template <typename TType> 
struct TColliderUnbindingType;

template <> struct TColliderUnbindingType<PDySphereColliderMetaInfo> { static constexpr EDyColliderType Value = EDyColliderType::Sphere; };
template <> struct TColliderUnbindingType<PDyCapsulesColliderMetaInfo> { static constexpr EDyColliderType Value = EDyColliderType::Capsule; };
template <> struct TColliderUnbindingType<PDyBoxColliderMetaInfo> { static constexpr EDyColliderType Value = EDyColliderType::Box; };

} /// ::dy namespace

#endif /// GUARD_DY_TYPE_PHYSICS_EDyColliderType_H
