#ifndef GUARD_DY_BUILTIN_ABSTRACT_ADYWIDGETMETARESOURCE_H
#define GUARD_DY_BUILTIN_ABSTRACT_ADYWIDGETMETARESOURCE_H
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

#include <string_view>
#include <Dy/Builtin/Interface/IDyResource.h>
#include <Dy/Core/Reflection/RDyBuiltinResources.h>

namespace dy
{

struct ADyWidgetMetaResource : public IDyResource
{
  virtual ~ADyWidgetMetaResource() = default;
  static constexpr auto value  = EDyResourceType::WidgetMeta;
  std::string_view mMetaBuffer = "";

  /// @brief Return meta buffer information.
  std::any GetMetaInfo() override final
  {
    return mMetaBuffer;
  };
};

} /// ::dy namespace

#endif /// GUARD_DY_BUILTIN_ABSTRACT_ADYWIDGETMETARESOURCE_H