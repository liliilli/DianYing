#ifndef GUARD_DY_BUILTIN_ABSTRACT_ADYSCRIPTRESOURCE_H
#define GUARD_DY_BUILTIN_ABSTRACT_ADYSCRIPTRESOURCE_H
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

#include <Dy/Builtin/Interface/IResource.h>
#include <Dy/Meta/Type/EResourceType.h>

namespace dy
{

/// @macro MDY_REGISTER_RESOURCE_SCRIPT
/// @brief Register cpp script source as builtin script resource.
/// @param __MAType__ Type for being enabled.
#define MDY_REGISTER_RESOURCE_SCRIPT(__MAType__, __MASpecifier__) \
  MDY_REGISTER_RESOURCE_WITH_SPECIFIER(__MAType__, __MASpecifier__); \
private: \
  std::any GetMetaInfo() override final { return 0; }; \
public: \
  class __ConstructionHelper final : public ::dy::IResource \
  { \
    using TScriptableFunction = ::dy::PDyScriptInstanceMetaInfo::TScriptableFunction; \
    using TFunctionReturn = std::invoke_result_t<TScriptableFunction>; \
    \
    template<typename TType> \
    static TFunctionReturn GetInstance() { return std::make_unique<TType>(); } \
    using  TSuper = __MAType__; \
    \
    inline static auto function = GetInstance<TSuper>; \
    \
    std::any GetMetaInfo() override final \
    { \
      using namespace dy; \
      PDyScriptInstanceMetaInfo metaInfo = {}; \
      metaInfo.mScriptType = EDyScriptType::Cpp; \
      if constexpr (IsInheritancedFrom<__MAType__, AWidgetCppScript> == true) \
      { metaInfo.mScriptMode = EDyScriptMode::Widget; } \
      else if constexpr (IsInheritancedFrom<__MAType__, AActorCppScript> == true) \
      { metaInfo.mScriptMode = EDyScriptMode::Actor; } \
      else if constexpr (IsInheritancedFrom<__MAType__, AGlobalCppScript> == true) \
      { metaInfo.mScriptMode = EDyScriptMode::Global; } \
      else { metaInfo.mScriptMode = EDyScriptMode::NoneError; } \
      \
      metaInfo.mSpecifierName = __MASpecifier__; \
      metaInfo.mBtInstantiationFunction = function; \
      return metaInfo; \
    } \
  }; \
private:

struct AScriptResource : public IResource
{
  static constexpr auto value = EResourceType::Script;
};

} /// ::dy namespace

#endif /// GUARD_DY_BUILTIN_ABSTRACT_ADYSCRIPTRESOURCE_H