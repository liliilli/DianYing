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

#include <memory>
#include <APlatformBase.h>
#include <Expr/ESuccess.h>

namespace dy
{

class AEngineBase
{
public:
  AEngineBase() = default;
  virtual ~AEngineBase() = 0;

  /// @brief Set platform information instance into engine.
  /// This function must be called only once, Otherwise causes assert.
  void SetPlatformInfo(std::unique_ptr<APlatformBase> info);

  /// @brief Store application command lines into container.
  virtual void StoreCommmandLines(int& argc, char**& argv) = 0;

  /// @brief Initialize Engine.
  virtual expr::EDySuccess Initialize() = 0;

  /// @brief Run Engine.
  virtual void Run() = 0;

  /// @brief Shutdown Engine.
  virtual expr::EDySuccess Shutdown() = 0;

  /// @brief Get platform information. This function call must be succeeded.
  APlatformBase& GetPlatformInfo() noexcept;

private:
  std::unique_ptr<APlatformBase> mPlatformInfo = nullptr;
};

inline AEngineBase::~AEngineBase() = default;

/// @brief Bootstrapping function.
extern AEngineBase* CreateEngine();
extern "C" std::unique_ptr<AEngineBase> gEngine;

} /// ::dy namespace