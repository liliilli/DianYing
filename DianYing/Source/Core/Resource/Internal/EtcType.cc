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
#include <Dy/Core/Resource/Internal/EtcType.h>

namespace dy
{

std::string PDyRendererConsturctionDescriptor::ToString()
{
  auto log = fmt::format(
R"dy(PDyRendererConsturctionDescriptor
Model Name : {}
)dy", this->mModelName);

  int32_t i = 0;
  for (const auto& materialName : this->mMaterialNames)
  {
    log += fmt::format("Material Name {} : {}\n", i, materialName);
    i++;
  }

  return log;
}

} /// ::dy namespace