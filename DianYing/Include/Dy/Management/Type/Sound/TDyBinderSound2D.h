#ifndef GUARD_DY_MANAGEMENT_TYPE_SOUND_TDyBinderSound2D_H
#define GUARD_DY_MANAGEMENT_TYPE_SOUND_TDyBinderSound2D_H
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
class FDyInstantSound2D;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

/// @class TDyBinderSound2D
/// @brief RAII Binder instance of sound 2d internal instance.
class TDyBinderSound2D final
{
public:
  TDyBinderSound2D() = default;
  TDyBinderSound2D(_MIN_ FDyInstantSound2D& iRefInstantSound2D);
  TDyBinderSound2D(_MIN_ const TDyBinderSound2D& p);
  TDyBinderSound2D& operator=(_MIN_ const TDyBinderSound2D& p);
  TDyBinderSound2D(_MIN_ TDyBinderSound2D&& p) noexcept;
  TDyBinderSound2D& operator==(_MIN_ TDyBinderSound2D&& p) noexcept;
  ~TDyBinderSound2D();

private:
  /// @brief
  FDyInstantSound2D* mPtrInstantSound2DInstance = nullptr;
  /// @brief
  MDY_TRANSIENT bool mIsCopied = false;
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_TYPE_SOUND_TDyBinderSound2D_H