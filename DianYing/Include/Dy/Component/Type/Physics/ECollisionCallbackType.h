#ifndef GUARD_DY_COMPONENT_TYPE_PHYSICS_EDyCollisionCbType_H
#define GUARD_DY_COMPONENT_TYPE_PHYSICS_EDyCollisionCbType_H
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

//!
//! Forward declaration
//!

namespace dy
{
struct TSignatureCollisionHit;
struct TSignatureCollisionOverlap;
}

//!
//! Implementation
//!

namespace dy
{

/// @enum   ECollisionCallbackType
/// @brief  Collision callback type.
enum class ECollisionCallbackType
{
  OnHit,
  OnOverlapBegin,
  OnOverlapEnd
};

// Base
template <ECollisionCallbackType> struct TCollisionCbTypeSignature;

template <> struct TCollisionCbTypeSignature<ECollisionCallbackType::OnHit> { using Type = TSignatureCollisionHit; };
template <> struct TCollisionCbTypeSignature<ECollisionCallbackType::OnOverlapBegin> { using Type = TSignatureCollisionOverlap; };
template <> struct TCollisionCbTypeSignature<ECollisionCallbackType::OnOverlapEnd> { using Type = TSignatureCollisionOverlap; };

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_TYPE_PHYSICS_EDyCollisionCbType_H