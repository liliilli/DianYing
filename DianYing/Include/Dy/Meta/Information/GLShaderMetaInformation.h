#ifndef GUARD_DY_META_INFORMATION_GLSHADERMETAINFORMATION_H
#define GUARD_DY_META_INFORMATION_GLSHADERMETAINFORMATION_H
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

#include <nlohmann/json_fwd.hpp>
#include <Dy/Meta/Information/CommonResourceMetaInfo.h>
#include <Dy/Meta/Type/EDyResourceTypes.h>

namespace dy
{

///
/// @struct PDyGLShaderInstanceMetaInfo
/// @brief  Instance meta information of `CDyShaderInformation` `CDyShaderResource_Deprecated` base type.
///
struct PDyGLShaderInstanceMetaInfo : public PDyCommonResourceMetaInfo
{
  struct DFragment final
  {
    std::string_view  mBuiltinBuffer    = MDY_INITIALIZE_EMPTYSTR;
    std::string       mExternalFilePath = MDY_INITIALIZE_EMPTYSTR;
  };

  std::string               mSpecifierName    = MDY_INITIALIZE_EMPTYSTR;
  std::array<DFragment, 6>  mShaderFragments  = {};
  bool                      mIsComputeShader  = false;

  MDY_NODISCARD DFragment& GetFragment(_MIN_ EDyShaderFragmentType type) noexcept
  {
    return this->mShaderFragments[static_cast<std::underlying_type_t<EDyShaderFragmentType>>(type)];
  }
  MDY_NODISCARD const DFragment& GetFragment(_MIN_ EDyShaderFragmentType type) const noexcept
  {
    return this->mShaderFragments[static_cast<std::underlying_type_t<EDyShaderFragmentType>>(type)];
  }
};

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyGLShaderInstanceMetaInfo& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyGLShaderInstanceMetaInfo& p);

} /// ::dy namespace

#endif /// GUARD_DY_META_INFORMATION_GLSHADERMETAINFORMATION_H