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

#include <cstdint>

namespace dy
{

/// @struct ABtResourceBase
/// @brief Builtin Resource Base type that supports RAII.
class ABtResourceBase
{
public:
  ABtResourceBase() = default;
  virtual ~ABtResourceBase() = 0;

  ABtResourceBase(const ABtResourceBase&) = delete;
  ABtResourceBase& operator=(const ABtResourceBase&) = delete;
  ABtResourceBase(ABtResourceBase&&) noexcept = default;
  ABtResourceBase& operator=(ABtResourceBase&&) noexcept = default;
  
  /// @brief Get buffer starting pointer.
  virtual void* GetBufferPtr() = 0;

  /// @brief Get the size of resource.
  virtual size_t GetResourceSize() = 0;
};

} /// ::dy namespace