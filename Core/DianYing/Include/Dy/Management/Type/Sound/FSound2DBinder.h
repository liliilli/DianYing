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
class FInstantSound2D;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

/// @class FSound2DBinder
/// @brief RAII Binder instance of sound 2d internal instance.
class FSound2DBinder final
{
public:
  FSound2DBinder() = default;
  FSound2DBinder(FInstantSound2D& iRefInstantSound2D);
  FSound2DBinder(const FSound2DBinder& p);
  FSound2DBinder& operator=(const FSound2DBinder& p);
  FSound2DBinder(FSound2DBinder&& p) noexcept;
  FSound2DBinder& operator==(FSound2DBinder&& p) noexcept;
  ~FSound2DBinder();

private:
  /// @brief Pointer of sound internal instance.
  FInstantSound2D* mPtrInstantSound2DInstance = nullptr;
  /// @brief Check this sound instance is copied so not valid when true.
  MDY_TRANSIENT bool mIsCopied = false;
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_TYPE_SOUND_TDyBinderSound2D_H