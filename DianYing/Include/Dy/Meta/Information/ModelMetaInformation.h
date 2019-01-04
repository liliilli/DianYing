#ifndef GUARD_DY_META_INFORMATION_MODELMETAINFORMATION_H
#define GUARD_DY_META_INFORMATION_MODELMETAINFORMATION_H
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

#include <Dy/Element/Interface/IDyToString.h>
#include <Dy/Meta/Information/CommonResourceMetaInfo.h>
#include <nlohmann/json_fwd.hpp>

//!
//! Forward declaration
//!

namespace dy
{
struct PDyModelConstructionVertexDescriptor_Deprecated;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

///
/// @struct PDyModelInstanceMetaInfo
/// @brief Model information construction descriptor
///
struct PDyModelInstanceMetaInfo_Deprecated final : public PDyCommonResourceMetaInfo, public IDyToString
{
  using TPtrBuffer = PDyModelConstructionVertexDescriptor_Deprecated*;
  std::string mSpecifierName     = MDY_INITIALIZE_EMPTYSTR;
  std::string mExternalModelPath      = MDY_INITIALIZE_EMPTYSTR;
  TPtrBuffer  mPtrBuiltinModelBuffer  = MDY_INITIALIZE_NULL;

  MDY_NODISCARD std::string ToString() override final
  {
    return fmt::format(R"dy(PDyModelInstanceMetaInfo\nModel Name : {}\nModel Path : {})dy", this->mSpecifierName, this->mExternalModelPath);
  }
};

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyModelInstanceMetaInfo_Deprecated& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyModelInstanceMetaInfo_Deprecated& p);

} /// ::dy namespace

#endif /// GUARD_DY_META_INFORMATION_MODELMETAINFORMATION_H