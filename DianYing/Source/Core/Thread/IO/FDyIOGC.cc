#include <precompiled.h>
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

/// Header file
#include <Dy/Core/Thread/IO/FDyIOGC.h>
#include <Dy/Management/MLog.h>
#include <Dy/Management/IO/MIORescInfo.h>
#include <Dy/Management/IO/MIOResource.h>

namespace dy
{

bool FDyIOGC::IsReferenceInstanceExist(const std::string& specifier, EResourceType type, EDyResourceStyle style)
{
  return ContainsIf(
    this->mRIGarbageCandidateList, 
    [&specifier, type, style](const auto& item) 
    { 
      const auto& iname  = item.mSpecifierName;
      const auto  itype  = item.mResourceType;
      const auto  istyle = item.mResourcecStyle;;
      return istyle == style && itype == type && iname == specifier;
    }
  );
}

std::optional<DDyIOReferenceInstance> 
FDyIOGC::MoveInstanceFromGC(const std::string& specifier, EResourceType type, EDyResourceStyle style)
{
  // Check nullility.
  auto it = std::find_if(
    MDY_BIND_BEGIN_END(this->mRIGarbageCandidateList), 
    [&specifier, type, style](const auto& item) 
    { 
      const auto& iname  = item.mSpecifierName;
      const auto  itype  = item.mResourceType;
      const auto  istyle = item.mResourcecStyle;;
      return istyle == style && itype == type && iname == specifier;
    }
  );
  if (it == this->mRIGarbageCandidateList.end()) { return std::nullopt; }

  // Get value and remove memory space of item in list.
  auto result = *it;
  FaseErase(this->mRIGarbageCandidateList, it);
  return std::move(result);
}

void FDyIOGC::InsertGcCandidate(DDyIOReferenceInstance iRICandidateList) noexcept
{
  this->mRIGarbageCandidateList.emplace_back(std::move(iRICandidateList));
}

void FDyIOGC::InsertGcCandidateList(const std::vector<DDyIOReferenceInstance>& iRICandidateList) noexcept
{
  this->mRIGarbageCandidateList.insert(
    this->mRIGarbageCandidateList.end(),
    MDY_BIND_BEGIN_END(iRICandidateList)
  );
}

EDySuccess FDyIOGC::TryGarbageCollectCandidateList() noexcept
{
  if (this->mRIGarbageCandidateList.empty() == true) { return DY_FAILURE; }

  while (this->mRIGarbageCandidateList.empty() == false)
  {
    decltype(mRIGarbageCandidateList) mGClist = this->mRIGarbageCandidateList;
    this->mRIGarbageCandidateList.clear();
    for (const auto& ri : mGClist)
    { // If garbase exist, detach from MIORescInfo & MIOResource.
      const auto& name  = ri.mSpecifierName;
      const auto type   = ri.mResourceType;
      const auto style  = ri.mResourcecStyle;

      switch (style)
      {
      case EDyResourceStyle::Information: 
      { // Try remove informaiton instance. This function call must be succeeded.
        MDY_CALL_ASSERT_SUCCESS(MIORescInfo::GetInstance().MDY_PRIVATE(TryRemove)(name, type));
      } break;
      case EDyResourceStyle::Resource: 
      { // Try remove resource instance. This funtion call must be succeeded.
        MDY_CALL_ASSERT_SUCCESS(MIOResource::GetInstance().MDY_PRIVATE(TryRemove)(name, type));
      } break;
      default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(DY_FAILURE);
      }
    }
  }

  return DY_SUCCESS;
}

} /// ::dy namespace