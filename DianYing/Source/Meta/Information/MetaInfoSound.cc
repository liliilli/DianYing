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

/// Header file
#include <Dy/Meta/Information/MetaInfoSound.h>
#include <Dy/Helper/Library/HelperJson.h>
#include <Dy/Helper/Library/HelperFilesystem.h>

namespace dy
{

/// @brief Serialization function
void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDySoundInstanceMetaInfo& p)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

/// @brief Deserialization function
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDySoundInstanceMetaInfo& p)
{
  p.mSourceType = EDyResourceSource::External;
  json::GetValueFromTo(j, "ExternalPath", p.mExternalPath);

  MDY_ASSERT_MSG_FORCE(IsFileExist(p.mExternalPath) == true, "Given sound path is not valid.");
}

} /// ::dy namespace