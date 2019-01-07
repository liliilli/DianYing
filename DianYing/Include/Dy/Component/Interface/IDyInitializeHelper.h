#ifndef GUARD_DY_COMPONENT_INTERFACE_IDYINITILIAZEHELPER_H
#define GUARD_DY_COMPONENT_INTERFACE_IDYINITILIAZEHELPER_H
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

namespace dy
{

///
/// @interface IDyInitializeHelper
/// @brief Initialize helper interface type.
///
template <typename TInitializeParam>
MDY_INTERFACE MDY_NO_VTABLE IDyInitializeHelper
{
  virtual ~IDyInitializeHelper() = default;

  ///
  /// @brief  Initialize derived class type manually.
  /// @param  descriptor  Descriptor instance which to initialize with.
  /// @return If successful, return DY_SUCCESS but DY_FAILURE.
  ///
  MDY_NODISCARD virtual EDySuccess Initialize(_MIN_ const TInitializeParam& descriptor) = 0;

  ///
  /// @brief Release instance manually.
  ///
  virtual void Release() = 0;
};

///
/// @interface IDyInitializeHelper
/// @brief Initialize helper interface type for non parameter-type.
///
template <>
MDY_INTERFACE MDY_NO_VTABLE IDyInitializeHelper<void>
{
  virtual ~IDyInitializeHelper() = default;

  ///
  /// @brief  Initialize derived class type manually.
  /// @return If successful, return DY_SUCCESS but DY_FAILURE.
  ///
  MDY_NODISCARD virtual EDySuccess Initialize() = 0;

  ///
  /// @brief Release instance manually.
  ///
  virtual void Release() = 0;
};


} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_INTERFACE_IDYINITILIAZEHELPER_H