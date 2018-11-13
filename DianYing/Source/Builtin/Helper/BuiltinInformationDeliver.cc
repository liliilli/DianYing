#include <precompiled.h>
///
/// MIT License
/// Copyright (c) 2018 Jongmin Yun
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
#include <Dy/Builtin/Helper/BuiltinInformationDeliver.h>
#include <Dy/Management/IO/MetaInfoManager.h>

namespace dy
{

EDySuccess FDyBuiltinInformationDeliver::ForwardWidgetMetaInformation(const std::string_view& metaString)
{
  auto& manager = MDyMetaInfo::GetInstance();
  return manager.pfAddWidgetMetaInformation(MSVSTR(metaString));
}

} /// ::dy namespace