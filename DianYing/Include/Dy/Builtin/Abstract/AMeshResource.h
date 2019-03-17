#ifndef GUARD_DY_BUILTIN_ABSTRACT_ADYMESHRESOURCE_H
#define GUARD_DY_BUILTIN_ABSTRACT_ADYMESHRESOURCE_H
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
#include <Dy/Meta/Information/MetaInfoModelMesh.h>

namespace dy
{

/// @macro MDY_REGISTER_RESOURCE_MESH
/// @brief Register mesh meta information source.
#define MDY_REGISTER_RESOURCE_MESH(__MAType__, __MASpecifierName__) \
  MDY_REGISTER_RESOURCE_WITH_SPECIFIER(__MAType__, __MASpecifierName__) \
  private: \
  void ConstructBuffer(_MOUT_ PDyMeshInstanceMetaInfo& property) noexcept; \
  public: \
  __MAType__() { this->ConstructBuffer(this->MIOMeta); }

struct AMeshResource : public IResource
{
protected:
  PDyMeshInstanceMetaInfo MIOMeta{};
public:
  static constexpr auto value = EResourceType::Mesh;

  /// @brief Get meta information that has mesh.
  std::any GetMetaInfo() override final;
};

} /// ::dy namespace

#endif /// GUARD_DY_BUILTIN_ABSTRACT_ADYMESHRESOURCE_H
