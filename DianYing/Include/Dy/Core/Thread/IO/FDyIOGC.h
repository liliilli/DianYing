#ifndef GUARD_DY_CORE_THREAD_IO_FDYIOGC_H
#define GUARD_DY_CORE_THREAD_IO_FDYIOGC_H
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

#include <Dy/Core/Reflection/RDyBuiltinResources.h>
#include <Dy/Core/Thread/IO/EDyIOTask.h>
#include <Dy/Core/Thread/IO/DDyIOReferenceInstance.h>

namespace dy
{

///
/// @struct FDyIOGC
/// @brief IO Instance Garbage collection manager
///
class FDyIOGC final
{
public:
  ///
  /// @brief Check whether resource's Reference Instance is exist on now as any kind of information.
  /// @param specifier Resource specifier name.
  /// @param type  Resource type.
  /// @param style Resource style mode.
  ///
  MDY_NODISCARD bool IsReferenceInstanceExist(_MIN_ const std::string& specifier, _MIN_ EDyResourceType type, _MIN_ EDyResourceStyle style);

  ///
  /// @brief
  ///
  MDY_NODISCARD void MoveInstanceFromGC(_MIN_ const std::string& speicifer, _MIN_ EDyResourceType type, _MIN_ EDyResourceStyle style);

  /// @brief Insert RI gc-ed candidate into container.
  void InsertGcCandidate(_MIN_ DDyIOReferenceInstance iRICandidateList) noexcept;
  /// @brief Insert RI gc-ed candidate list into container.
  void InsertGcCandidateList(_MIN_ const std::vector<DDyIOReferenceInstance>& iRICandidateList) noexcept;

  /// @brief 
  EDySuccess TryGarbageCollectCandidateList() noexcept;

private:
  /// @brief
  std::vector<DDyIOReferenceInstance> mRIGarbageCandidateList = {};
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_THREAD_IO_FDYIOGC_H