#ifndef GUARD_DY_CORE_RESOURCE_RESOURCE_FDYMODELRESOURCE_H
#define GUARD_DY_CORE_RESOURCE_RESOURCE_FDYMODELRESOURCE_H
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

#include <Dy/Core/Resource/Resource/FDyMeshResource.h>
#include <Dy/Core/Resource/Type/TDyInformationBinder.h>
#include <Dy/Core/Resource/Type/TDyResourceBinder.h>

namespace dy
{

///
/// @class FDyModelResource
/// @brief Model resource class that serve heap instance.
///
class FDyModelResource final
{
public:
  FDyModelResource(_MINOUT_ const FDyModelInformation& input);
  ~FDyModelResource() = default;

  /// @brief Get specifier name of model resource.
  MDY_NODISCARD const std::string& GetSpecifierName() const noexcept
  {
    return this->mSpecifierName;
  }

  /// @brief Get valid mesh resource list, not modifiable.
  MDY_NODISCARD const auto& GetMeshResourceList() const noexcept
  {
    return this->mMeshResource;
  }

private:
  std::string                   mSpecifierName  = MDY_INITIALIZE_EMPTYSTR;
  TDyIInformationBinderModel    mBinderInformation;
  std::vector<std::unique_ptr<TDyIResourceBinderMesh>> mMeshResource = {};
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_RESOURCE_FDYMODELRESOURCE_H