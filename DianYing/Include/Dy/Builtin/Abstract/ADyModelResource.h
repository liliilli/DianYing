#ifndef GUARD_DY_BUILTIN_ABSTRACT_ADYMODELRESOURCE_H
#define GUARD_DY_BUILTIN_ABSTRACT_ADYMODELRESOURCE_H
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
#include <Dy/Builtin/Interface/IDyResource.h>
#include <Dy/Core/Reflection/RDyBuiltinResources.h>
#include <Dy/Core/Resource/Internal/ModelType.h>
#include <Dy/Meta/Information/MetaInfoModel.h>

namespace dy
{
  
/// @macro MDY_REGISTER_RESOURCE_MODEL
/// @brief Register model meta information source.
#define MDY_REGISTER_RESOURCE_MODEL(__MAType__, __MASpecifierName__) \
  MDY_REGISTER_RESOURCE_WITH_SPECIFIER(__MAType__, __MASpecifierName__) \
  private: \
  void ConstructBuffer(_MOUT_ PDyModelInstanceMetaInfo& buffer) noexcept; \
  public: \
  __MAType__() { ConstructBuffer(this->mMetaInfo); }

struct ADyModelResource : public IDyResource
{
protected:
  PDyModelInstanceMetaInfo mMetaInfo = {};
public:
  virtual ~ADyModelResource() = default;
  static constexpr auto value = EDyResourceType::Model;

  /// @brief Return meta info, that has buffer pointer of model.
  std::any GetMetaInfo() override final;
};

} /// ::dy namespace

#endif /// GUARD_DY_BUILTIN_ABSTRACT_ADYMODELRESOURCE_H