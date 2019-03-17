#ifndef GUARD_DY_BUILTIN_ABSTRACT_ADYWIDGETMETARESOURCE_H
#define GUARD_DY_BUILTIN_ABSTRACT_ADYWIDGETMETARESOURCE_H
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

#include <string_view>
#include <Dy/Builtin/Interface/IResource.h>
#include <Dy/Meta/Type/EResourceType.h>

namespace dy
{

struct AWidgetMetaResource : public IResource
{
  virtual ~AWidgetMetaResource() = default;
  static constexpr auto value  = EResourceType::WidgetMeta;
  std::string_view mMetaBuffer = "";

  /// @brief Return meta buffer information.
  std::any GetMetaInfo() override final
  {
    return mMetaBuffer;
  };
};

} /// ::dy namespace

#endif /// GUARD_DY_BUILTIN_ABSTRACT_ADYWIDGETMETARESOURCE_H