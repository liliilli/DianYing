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
#include <Dy/Core/Thread/IO/FRescIOGC.h>
#include <Dy/Management/MLog.h>
#include <Dy/Management/IO/MIORescInfo.h>
#include <Dy/Management/IO/MIOResource.h>

namespace dy
{

bool FRescIOGC::IsReferenceInstanceExist(const std::string& specifier, EResourceType type, EResourceStyle style)
{
  return ContainsIf(
    this->mRIGarbageCandidateList, 
    [&specifier, type, style](const auto& item) 
    { 
      const auto& iname  = item->mSpecifierName;
      const auto  itype  = item->mResourceType;
      const auto  istyle = item->mResourcecStyle;;
      return istyle == style && itype == type && iname == specifier;
    }
  );
}

bool FRescIOGC::IsGcCandidateExist() const noexcept
{
  return this->mRIGarbageCandidateList.empty() == false;
}

std::unique_ptr<DIOReferenceInstance> 
FRescIOGC::MoveInstanceFromGC(const std::string& specifier, EResourceType type, EResourceStyle style)
{
  // Check nullility.
  const auto it = std::find_if(
    MDY_BIND_BEGIN_END(this->mRIGarbageCandidateList), 
    [&specifier, type, style](const auto& item) 
    { 
      const auto& iname  = item->mSpecifierName;
      const auto  itype  = item->mResourceType;
      const auto  istyle = item->mResourcecStyle;;
      return istyle == style && itype == type && iname == specifier;
    }
  );
  if (it == this->mRIGarbageCandidateList.end()) { return nullptr; }

  // Get value and remove memory space of item in list.
  auto result = std::move(*it);
  FaseErase(this->mRIGarbageCandidateList, it);
  return result;
}

void FRescIOGC::InsertGcCandidate(std::unique_ptr<DIOReferenceInstance>& ioRICandidateList) noexcept
{
  this->mRIGarbageCandidateList.emplace_back(std::move(ioRICandidateList));
}

void FRescIOGC::InsertGcCandidateList(std::vector<std::unique_ptr<DIOReferenceInstance>> iRICandidateList) noexcept
{
  for (auto& smtptrGcedRiItem : iRICandidateList)
  {
    this->mRIGarbageCandidateList.emplace_back(std::move(smtptrGcedRiItem));
  }
  //this->mRIGarbageCandidateList.insert(this->mRIGarbageCandidateList.end(), MDY_BIND_BEGIN_END(iRICandidateList));
}

EDySuccess FRescIOGC::TryGarbageCollectCandidateList() 
{
  if (this->mRIGarbageCandidateList.empty() == true) { return DY_FAILURE; }

  // We use while statement intentionally because resource deletion of GC candidate
  // cause the insertion of dependent resource GC candidate into list.
  while (this->mRIGarbageCandidateList.empty() == false)
  {
    decltype(mRIGarbageCandidateList) mGClist = std::move(this->mRIGarbageCandidateList);
    this->mRIGarbageCandidateList.clear();

    for (const auto& ri : mGClist)
    { 
      // If garbage is exist, detach from MIORescInfo & MIOResource.
      const auto& name = ri->mSpecifierName;
      const auto type  = ri->mResourceType;
      const auto style = ri->mResourcecStyle;

      switch (style)
      {
      case EResourceStyle::Information: 
      { // Try remove informaiton instance. This function call must be succeeded.
        MDY_CALL_ASSERT_SUCCESS(MIORescInfo::GetInstance().MDY_PRIVATE(TryRemove)(name, type));
      } break;
      case EResourceStyle::Resource: 
      { // Try remove resource instance. This funtion call must be succeeded.
        MDY_CALL_ASSERT_SUCCESS(MIOResource::GetInstance().MDY_PRIVATE(TryRemove)(name, type));
      } break;
      default: MDY_UNEXPECTED_BRANCH(); throw;
      }
    }
  }

  return DY_SUCCESS;
}

} /// ::dy namespace