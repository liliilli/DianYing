#ifndef GUARD_DY_CORE_REFLECTION_INLINE_RBUILTINRESOURCETMP_INL
#define GUARD_DY_CORE_REFLECTION_INLINE_RBUILTINRESOURCETMP_INL
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

#include <Dy/Helper/System/TypeTraits.h>
#include <Dy/Builtin/Abstract/ALoadingWidgetMetaResource.h>
#include <Dy/Builtin/Abstract/ALoadingBootResource.h>
#include <Dy/Builtin/Abstract/ALoadingGlobalResource.h>

namespace dy::reflect
{
  
template <typename TType>
void RBuiltinResources::RegisterScriptResource(const std::string& specifier)
{
  if constexpr (IsBaseClassOf<TType, ALoadingBootResource> == true)
  { // Bind boot resource script to container.
    GetBootResourceMetaInfo() = std::make_pair(TType::value, &__Rfc__GetInstance<TType>);
  }
  else if constexpr (IsBaseClassOf<TType, ALoadingGlobalResource> == true)
  { // Add global loading resource script to container.
    GetGlobalResourceMetaInfo().emplace_back(TType::value, &__Rfc__GetInstance<TType>);
  }
  else
  { // Bind general (widget, actor) script to container.
    GetResourceMapReference().try_emplace(
      specifier, 
      TType::value, &__Rfc__GetInstance<TType::__ConstructionHelper>);
  }
}

template <typename TType>
RDyBuiltinResourceRegister<TType>::RDyBuiltinResourceRegister(const std::string_view& name)
{
  // Need to avoid duplicated initiation.
  if (mFlag == true) { return; }

  static_assert(
    IsInheritancedFrom<TType, IResource> == true, 
    "TType must be IResource derived type.");

  // This dirty avoidance will be cleaned by using uuid... (weep)
  std::string specifier = name.data(); specifier += std::to_string(count);

  if constexpr (TType::value == EResourceType::Script) 
  { 
    this->RegisterScriptResource<TType>(specifier); 
  }
  else if constexpr (TType::value == EResourceType::RenderPipeline
                  || TType::value == EResourceType::RenderItem)
  {
    GetResourceMapReference().try_emplace(
    specifier, 
    TType::value, &__Rfc__GetInstance<TType::__ConstructionHelper>);
  }
  else if constexpr (IsInheritancedFrom<TType, ALoadingWidgetMetaResource> == true)
  { 
    GetLoadingWidgetResourceMetaInfo() = std::make_pair(TType::value, &__Rfc__GetInstance<TType>); 
  }
  else 
  { 
    GetResourceMapReference().try_emplace(specifier, TType::value, &__Rfc__GetInstance<TType>); 
  }

  count += 1;
  mFlag = true;
}

} /// ::dy::reflect namespace

#endif /// GUARD_DY_CORE_REFLECTION_INLINE_RBUILTINRESOURCETMP_INL