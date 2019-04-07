#ifndef GUARD_DY_ELEMENT_INTERFACE_IDYCOLLIABLE_H
#define GUARD_DY_ELEMENT_INTERFACE_IDYCOLLIABLE_H
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
class IColliable;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

/// @class IColliable
/// @brief Colliable object interface which gets function.
class IColliable
{
public:
  virtual ~IColliable() = 0;

  /// @brief Callback function of collider enter but both are collidable.
  virtual void OnCollisionEnter(CCollider* collider) {};

  /// @brief Callback function of collider stay but both are collidable.
  virtual void OnCollisionStay(CCollider* collider) {};

  /// @brief Callback function of collider exit but both are collidable.
  virtual void OnCollisionExit(CCollider* collider) {};

  /// @brief Callback function of trigger enter.
  virtual void OnTriggerEnter(CCollider* collider) {};

  /// @brief Callback function of trigger stay.
  virtual void OnTriggerStay(CCollider* collider) {};

  /// @brief Callback function of trigger exit.
  virtual void OnTriggerExit(CCollider* collider) {};
};

inline IColliable::~IColliable() = default;

} /// ::dy namespace

#endif /// GUARD_DY_ELEMENT_INTERFACE_IDYCOLLIABLE_H