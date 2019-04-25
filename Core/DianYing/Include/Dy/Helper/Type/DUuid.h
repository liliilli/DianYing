#ifndef GUARD_DY_HELPER_TYPE_UUID_H
#define GUARD_DY_HELPER_TYPE_UUID_H
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
#include <string>
#include <Math/Type/Micellanous/DUuid.h>
#include <nlohmann/json_fwd.hpp>

namespace dy
{

using math::DUuid;

} /// ::dy namespace

namespace dy::math
{

void to_json(nlohmann::json& oJson, const DUuid& iMeta);
void from_json(const nlohmann::json& iJson, DUuid& oMeta);

}

#endif /// GUARD_DY_HELPER_TYPE_UUID_H