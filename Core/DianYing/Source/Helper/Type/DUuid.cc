#include <precompiled.h>
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

#include <Dy/Helper/Type/DUuid.h>
#include <Dy/Management/MLog.h>

namespace dy::math
{

void to_json(nlohmann::json& oJson, const DUuid& iUuid)
{
  oJson = iUuid.ToString();
}

void from_json(const nlohmann::json& iJson, DUuid& oUuid)
{
  const auto resultUuid = DUuid(iJson.get<std::string>());
  oUuid = resultUuid;
  //std::memcpy(&oUuid.mUuid, resultUuid., oUuid.Size());
}

} /// ::dy namespace
