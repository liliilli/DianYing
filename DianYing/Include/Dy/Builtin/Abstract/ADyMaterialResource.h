#ifndef GUARD_DY_BUILTIN_ABSTACT_ADYMATERIALRESOURCE_H
#define GUARD_DY_BUILTIN_ABSTACT_ADYMATERIALRESOURCE_H
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

#include <Dy/Builtin/Interface/IDyResource.h>
#include <Dy/Core/Reflection/RDyBuiltinResources.h>
#include <Dy/Meta/Information/MetaInfoMaterial.h>

namespace dy
{

struct ADyMaterialResource : public IDyResource
{
protected:
  PDyMaterialInstanceMetaInfo mMetaInfo{};
public:
  static constexpr auto value = EDyResourceType::Material;

  /// @brief Get material meta information.
  std::any GetMetaInfo() override final { return this->mMetaInfo; }
};

} /// ::dy namespace

#endif /// GUARD_DY_BUILTIN_ABSTACT_ADYMATERIALRESOURCE_H