#ifndef GUARD_DY_CORE_COMPONENT_INTERNAL_MODEL_TYPE_H
#define GUARD_DY_CORE_COMPONENT_INTERNAL_MODEL_TYPE_H
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

#include <string>
#include <Dy/Core/Component/Interface/IDyToString.h>

namespace dy
{

///
/// @struct PDyModelConstructionDescriptor
/// @brief Model information construction descriptor
///
struct PDyModelConstructionDescriptor final : public IDyToString
{
  std::string                           mModelName        = "";
  std::string                           mModelPath        = "";

  [[nodiscard]]
  std::string ToString() override final
  {
    return fmt::format(
R"dy(PDyModelConstructionDescriptor
Model Name : {}
Model Path : {}
)dy", this->mModelName, this->mModelPath);
  };
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_COMPONENT_INTERNAL_MODEL_TYPE_H