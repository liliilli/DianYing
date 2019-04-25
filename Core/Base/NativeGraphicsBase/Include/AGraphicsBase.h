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

#include <EGraphicsApi.h>

namespace dy
{

class AGraphicsBase
{
public:
  AGraphicsBase() = default;

  AGraphicsBase(const AGraphicsBase&) = default;
  AGraphicsBase& operator=(const AGraphicsBase&) = default;
  AGraphicsBase(AGraphicsBase&&) = default;
  AGraphicsBase& operator=(AGraphicsBase&&) = default;

  virtual ~AGraphicsBase() = 0;

  /// @brief Get graphics API value.
  EXPR_E(EGraphicsApi) GetGraphicsApi() const noexcept;

  /// @brief Get the string of graphics API value using.
  std::string GetGraphicsApiString() const noexcept;

protected:
  EXPR_E(EGraphicsApi) mApiType = EGraphicsApi::OpenGL;
};

} /// ::dy namespace