#ifndef GUARD_DY_MANAGEMENT_INTERNAL_PHYSICS_CollsionResults_H
#define GUARD_DY_MANAGEMENT_INTERNAL_PHYSICS_CollsionResults_H
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

#include <Dy/Helper/Type/DVector3.h>

namespace dy
{

/// @struct FDyHitResult
/// @brief Hit result instance for collision callback handling (just hit).
struct FDyHitResult final
{
  /// @brief specifies contact position between two physics shapes.
  DVector3 mContactPosition{};
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_INTERNAL_PHYSICS_CollsionResults_H