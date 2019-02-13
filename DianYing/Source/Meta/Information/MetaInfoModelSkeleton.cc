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
#include <Dy/Meta/Information/MetaInfoModelSkeleton.h>
#include <Dy/Helper/Library/HelperJson.h>

namespace dy
{

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyModelSkelInstanceMetaInfo& p)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyModelSkelInstanceMetaInfo& p)
{
  p.mSourceType = decltype(p.mSourceType)::External;
  DyJsonGetValueFromTo(j, "ExternalPath", p.mExternalPath);
  DyJsonGetValueFromTo(j, "IsCompressed", p.mIsCompressed);
}

} /// ::dy namespace