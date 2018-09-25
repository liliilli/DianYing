#ifndef GUARD_DY_CORE_COMPONENT_INTERNAL_ETC_TYPE_H
#define GUARD_DY_CORE_COMPONENT_INTERNAL_ETC_TYPE_H
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
#include <vector>
#include <Dy/Element/Interface/IDyToString.h>

namespace dy
{

///
/// @struct PDyRendererConsturctionDescriptor
/// @brief
///
struct PDyRendererConsturctionDescriptor final : public IDyToString
{
  std::string               mModelName      = "";
  std::vector<std::string>  mMaterialNames  = {};

  ///
  /// PDyRendererConsturctionDescriptor
  /// Model Name : ""
  /// Material Name 0 : ""
  /// ...
  /// Material Name N : ""
  ///
  [[nodiscard]]
  std::string ToString() override final;
};

///
/// @struct PDySoundConstructionDescriptor
/// @brief Sound file information descriptor
///
struct PDySoundConstructionDescriptor final
{
  std::string mSoundName = "";
  std::string mSoundPath = "";
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_COMPONENT_INTERNAL_ETC_TYPE_H