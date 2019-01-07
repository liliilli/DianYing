#ifndef GUARD_DY_BUILTIN_ABSTRACT_ADYLOADINGBOOTRESOURCE_H
#define GUARD_DY_BUILTIN_ABSTRACT_ADYLOADINGBOOTRESOURCE_H
///
/// MIT License
/// Copyright (c) 2018-2019 Jongmin Yun
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.
///

#include <Dy/Builtin/Abstract/ADyScriptResource.h>
#include <Dy/Meta/Type/DDyResourceName.h>

//!
//! Forward declaration
//!

namespace dy::reflect
{
class RDyBuiltinResource;
} /// ::dy::reflect namespace

//!
//! Implementation
//!

namespace dy
{

struct ADyLoadingBootResource : public ADyScriptResource
{
  MDY_NOTUSED std::any GetMetaInfo() override final { return 0; }

protected:
  std::vector<DDyResourceName> mResourceRequisitionList = {};

private:
  /// @brief Get boot resource meta list.
  const auto& GetResourceMetaList() const noexcept { return this->mResourceRequisitionList; }

  friend class ::dy::reflect::RDyBuiltinResource;
};

} /// ::dy namespace

#endif /// GUARD_DY_BUILTIN_ABSTRACT_ADYLOADINGBOOTRESOURCE_H
