#ifndef GUARD_DY_BUILTIN_ABSTACT_ADYMATERIALRESOURCE_H
#define GUARD_DY_BUILTIN_ABSTACT_ADYMATERIALRESOURCE_H
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
#include <Dy/Core/Reflection/RBuiltinResources.h>
#include <Dy/Meta/Information/MetaInfoMaterial.h>

namespace dy
{

/// @macro MDY_REGISTER_RESOURCE_MATERIAL
/// @brief Register material meta information source.
#define MDY_REGISTER_RESOURCE_MATERIAL(__MAType__, __MASpecifier__) \
  MDY_REGISTER_RESOURCE_WITH_SPECIFIER(__MAType__, __MASpecifier__) \
  private: \
    void ConstructBuffer(_MOUT_ PDyMaterialInstanceMetaInfo& property) noexcept; \
  public: \
  __MAType__() \
  { \
    ConstructBuffer(this->MIOMeta); \
  }

struct AMaterialResource : public IResource
{
protected:
  PDyMaterialInstanceMetaInfo MIOMeta{};
public:
  static constexpr auto value = EResourceType::Material;

  /// @brief Get material meta information.
  std::any GetMetaInfo() override final { return this->MIOMeta; }
};

} /// ::dy namespace

#endif /// GUARD_DY_BUILTIN_ABSTACT_ADYMATERIALRESOURCE_H