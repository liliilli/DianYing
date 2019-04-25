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

/// Header file
#include <AGraphicsBase.h>

namespace dy
{

inline AGraphicsBase::~AGraphicsBase() = default;

EXPR_E(EGraphicsApi) AGraphicsBase::GetGraphicsApi() const noexcept
{
  return this->mApiType;
}

std::string AGraphicsBase::GetGraphicsApiString() const noexcept
{
  return EGraphicsApi::ToString(this->mApiType);
}

} /// ::dy namespace