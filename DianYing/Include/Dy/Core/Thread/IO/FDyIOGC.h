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

#include <Dy/Core/Reflection/RBuiltinResources.h>
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
  /// @brief Check whether resource's Reference Instance is exist on now as any kind of information.
  /// @param specifier Resource specifier name.
  /// @param type  Resource type.
  /// @param style Resource style mode.
  MDY_NODISCARD bool IsReferenceInstanceExist(const std::string& specifier, EResourceType type, EResourceStyle style);

  /// @brief Move instance from gc list as return value.
  MDY_NODISCARD std::optional<std::unique_ptr<DDyIOReferenceInstance>> 
    MoveInstanceFromGC(const std::string& speicifer, EResourceType type, EResourceStyle style);

  /// @brief Insert RI gc-ed candidate into container.
  /// Inserted argument will be empty.
  void InsertGcCandidate(std::unique_ptr<DDyIOReferenceInstance>& ioRICandidateList) noexcept;
  /// @brief Insert RI gc-ed candidate list into container.
  void InsertGcCandidateList(std::vector<std::unique_ptr<DDyIOReferenceInstance>> iRICandidateList) noexcept;

  /// @brief 
  EDySuccess TryGarbageCollectCandidateList() noexcept;

private:
  /// @brief
  std::vector<std::unique_ptr<DDyIOReferenceInstance>> mRIGarbageCandidateList = {};
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_THREAD_IO_FDYIOGC_H