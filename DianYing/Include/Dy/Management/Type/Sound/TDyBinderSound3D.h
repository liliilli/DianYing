#ifndef GUARD_DY_MANAGEMENT_TYPE_SOUND_TDyBinderSound3D_H
#define GUARD_DY_MANAGEMENT_TYPE_SOUND_TDyBinderSound3D_H
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
class FDyInstantSound3D;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

/// @class TDyBinderSound3D
/// @brief RAII Binder instance of sound 3D internal instance.
class TDyBinderSound3D final
{
public:
  TDyBinderSound3D() = default;
  TDyBinderSound3D(_MIN_ FDyInstantSound3D& iRefInstantSound2D);
  TDyBinderSound3D(_MIN_ const TDyBinderSound3D& p);
  TDyBinderSound3D& operator=(_MIN_ const TDyBinderSound3D& p);
  TDyBinderSound3D(_MIN_ TDyBinderSound3D&& p) noexcept;
  TDyBinderSound3D& operator==(_MIN_ TDyBinderSound3D&& p) noexcept;
  ~TDyBinderSound3D();

private:
  /// @brief The pointer of internal instance.
  FDyInstantSound3D* mPtrInstantSound3DInstance = nullptr;
  /// @brief If this is false, destructor and logic does not call stop logic of internal instance.
  MDY_TRANSIENT bool mIsCopied = false;
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_TYPE_SOUND_TDyBinderSound3D_H
