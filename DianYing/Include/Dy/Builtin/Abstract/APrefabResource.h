#ifndef GUARD_DY_BUILTIN_ABSTRACT_APREFABRESOURCE_H
#define GUARD_DY_BUILTIN_ABSTRACT_APREFABRESOURCE_H
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
  
/// @macro MDY_REGISTER_RESOURCE_PREFAB
/// @brief Register model meta information source.
#define MDY_REGISTER_RESOURCE_PREFAB(__MAType__, __MASpecifierName__) \
  MDY_REGISTER_RESOURCE_WITH_SPECIFIER(__MAType__, __MASpecifierName__); \
  public: \
  __MAType__();

struct APrefabResource : public IResource
{
protected:
  std::string_view mJsonPrefab;
public:
  virtual ~APrefabResource() = default;
  static constexpr auto value = EResourceType::Prefab;

  /// @brief Return meta info, that has buffer pointer of model.
  std::any GetMetaInfo() override final;
};

} /// ::dy namespace

#endif /// GUARD_DY_BUILTIN_ABSTRACT_APREFABRESOURCE_H