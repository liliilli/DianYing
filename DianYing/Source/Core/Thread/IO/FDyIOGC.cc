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
#include <Dy/Management/LoggingManager.h>
#include <Dy/Management/IO/MDyIOData.h>
#include <Dy/Management/IO/MDyIOResource.h>

namespace dy
{

bool FDyIOGC::IsReferenceInstanceExist(_MIN_ const std::string& specifier, _MIN_ EDyResourceType type, _MIN_ EDyResourceStyle style)
{
  return false;
}

void FDyIOGC::InsertGcCandidate(DDyIOReferenceInstance iRICandidateList) noexcept
{
  this->mRIGarbageCandidateList.emplace_back(std::move(iRICandidateList));
}

void FDyIOGC::InsertGcCandidateList(const std::vector<DDyIOReferenceInstance>& iRICandidateList) noexcept
{
  this->mRIGarbageCandidateList.insert(this->mRIGarbageCandidateList.end(), MDY_BIND_BEGIN_END(iRICandidateList));
}

EDySuccess FDyIOGC::TryGarbageCollectCandidateList() noexcept
{
  if (this->mRIGarbageCandidateList.empty() == true) { return DY_FAILURE; }

  for (const auto& ri : this->mRIGarbageCandidateList)
  { // If garbase exist, detach from MDyIOData & MDyIOResource.
    const auto& name  = ri.mSpecifierName;
    const auto type   = ri.mResourceType;
    const auto style  = ri.mResourcecStyle;

    switch (style)
    {
    case EDyResourceStyle::Information: 
    { // Try remove informaiton instance. This function call must be succeeded.
      MDY_CALL_ASSERT_SUCCESS(MDyIOData::GetInstance().MDY_PRIVATE_SPECIFIER(TryRemove)(name, type));
    } break;
    case EDyResourceStyle::Resource: 
    { // Try remove resource instance. This funtion call must be succeeded.
      MDY_CALL_ASSERT_SUCCESS(MDyIOResource::GetInstance().MDY_PRIVATE_SPECIFIER(TryRemove)(name, type));
    } break;
    default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(DY_FAILURE);
    }

  }
  this->mRIGarbageCandidateList.clear();
  return DY_SUCCESS;
}

} /// ::dy namespace