#ifndef GUARD_DY_MANAGEMENT_HELPER_PHYSXERRORCALLBACK_H
#define GUARD_DY_MANAGEMENT_HELPER_PHYSXERRORCALLBACK_H
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

#include <foundation/PxErrorCallback.h>

namespace dy
{

/// @class FPhysXErrorCallback
/// @brief PhysX internal error delegator type.
class FPhysXErrorCallback final : public physx::PxErrorCallback
{
public:
  /// @brief error callback function.
  void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line) override final;
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_HELPER_PHYSXERRORCALLBACK_H