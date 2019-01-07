#ifndef GUARD_DY_META_INFORMATION_METAINFOMODEL_H
#define GUARD_DY_META_INFORMATION_METAINFOMODEL_H
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

#include <nlohmann/json_fwd.hpp>
#include <Dy/Meta/Information/CommonResourceMetaInfo.h>

namespace dy
{
  
///
/// @struct PDyModelInstanceMetaInfo
/// @brief Descriptor instance which saves information to create model meshes.
///
struct PDyModelInstanceMetaInfo final : public PDyCommonResourceMetaInfo
{
  using TBuiltinMeshSpecifierList = std::vector<std::string>;

  std::string               mSpecifierName            = MDY_INITIALIZE_EMPTYSTR;
  TBuiltinMeshSpecifierList mBuiltinMeshSpecifierList = {};
  bool                      mIsUsingBuiltinMesh       = false;

  [[maybe_unused]] std::string   mExternalModelName  = MDY_INITIALIZE_EMPTYSTR;
};

} /// ::dy namespace

#endif /// GUARD_DY_META_INFORMATION_METAINFOMODEL_H