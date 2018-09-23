#ifndef CORE_COMPONENT_INTERFACE_ISCRIPTABLE_H
#define CORE_COMPONENT_INTERFACE_ISCRIPTABLE_H
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

#include <Dy/Core/Component/Interface/IDyUpdatable.h>

//!
//! Implementation
//!

namespace dy {

///
/// @class IDyScriptable
/// @brief
/// The most base script class, which is derived by user-defined script class.
/// ScriptFrame inherits component::CComponent class the most base class of
/// all components.
///
/// All derived class must be a final,
/// and define Initiate(), Start() and Update() function and many callbacks.
///
/// Initiate() and Start() will be called only once in initialization time,
/// and prior to first Update() call.
///
class IDyScriptable : public IDyUpdatable
{
public:
  virtual ~IDyScriptable() = default;

  ///
  /// @brief
  /// This method will be called only one time, when ScriptFrame is attached to
  /// object, and actived first time in game applicaiton.
  ///
  /// This is just optional, you can do not to override Initiate() method
  /// when there is no need.
  ///
  virtual void Initiate() {};

  ///
  /// @brief
  /// This method must be called in Initialization time of script bound to object.
  /// And all derived scripts have to override this,
  /// even though not have components to set up.
  ///
  virtual void Start() {};

  ///
  /// @brief
  /// This method updates information of script instance bound to specific object.
  /// Must be overriden and implemented by derived script class.
  ///
  void Update(float dt) override {};

  ///
  /// @brief
  /// This method will be called only one time, when the object are destroyed.
  /// Object's destuctor call all destroy() methods in all scripts bound to object to be vanished.
  /// This is optional method, you could leave it vacant.
  ///
  virtual void Destroy() {};

  ///
  /// @brief
  /// This method will be called
  /// if only bound object is activated from deactivation.
  ///
  virtual void OnEnabled() {};

  ///
  /// @brief
  /// This method will be called
  /// If only bound object is desabled from activation.
  ///
  virtual void OnDisabled() {};

private:
  /// internal flag for OnStart() funciton.
  mutable bool mIsScriptStartedDirty = true;
};

} /// ::dy namespace

#endif /// CORE_COMPONENT_INTERFACE_ISCRIPTABLE_H
