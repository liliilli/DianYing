#pragma once
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

#include <ABtResourceBase.h>
#include <Windows.h>
#include <XDisableWinMacro.h>

namespace dy
{

/// @struct FBtResourceHandle
/// @brief Builtin Resource Base type that supports RAII.
class FBtResourceHandle : public ABtResourceBase
{
public:
  FBtResourceHandle(HRSRC validResource);
  virtual ~FBtResourceHandle();

  FBtResourceHandle(FBtResourceHandle&& from) noexcept;
  FBtResourceHandle& operator=(FBtResourceHandle&& from) noexcept;
  
  /// @brief Get buffer starting pointer.
  void* GetBufferPtr() override final;

  /// @brief Get the size of resource.
  size_t GetResourceSize() override final;

private:
  HRSRC   mValidResource = nullptr;
  HGLOBAL mResourceHnd = nullptr;
};

} /// ::dy namespace