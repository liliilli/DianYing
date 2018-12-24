#include <precompiled.h>
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

/// Header file
#include <Dy/Core/Thread/IO/FDyIOGC.h>
#include <Dy/Management/LoggingManager.h>

namespace dy
{

bool FDyIOGC::IsReferenceInstanceExist(_MIN_ const std::string& specifier, _MIN_ EDyResourceType type, _MIN_ EDyResourceStyle style)
{
  return false;
}

void FDyIOGC::InsertGcCandidateList(const std::vector<DDyIOReferenceInstance>& iRICandidateList) noexcept
{
  this->mRIGarbageCandidateList.insert(this->mRIGarbageCandidateList.end(), MDY_BIND_BEGIN_END(iRICandidateList));
}

EDySuccess FDyIOGC::TryGarbageCollectCandidateList() noexcept
{
  if (this->mRIGarbageCandidateList.empty() == false) { return DY_FAILURE; }

  for (const auto& ri : this->mRIGarbageCandidateList)
  {
    MDY_NOT_IMPLEMENTED_ASSERT();
  }
  this->mRIGarbageCandidateList.clear();
  return DY_SUCCESS;
}

} /// ::dy namespace