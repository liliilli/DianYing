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

#include <Dy/Builtin/Font/Gothic.h>
#include <Dy/Resource/resource1.h>

namespace dy::builtin
{

void FBtFtDefaultGothic::ConstructBuffer(PBuiltinMetaFontInfo& oInfo) noexcept
{
  oInfo.mIdentifier = this->sName;
  oInfo.mUuid       = DUuid("204c0ab2-b0ab-422e-9b69-43a716ce802d");

  auto& details = oInfo.mDetails;

  details.mFontType = decltype(details.mFontType)::SDF;
  details.mFontInfoType = PDyMetaFontInformation::DBuiltin::EBuffer::Plain;
  details.mFontInfoBuffer = 
#include "Gothic.info"
    ;

  details.mTextureType = PDyMetaFontInformation::DBuiltin::EBuffer::Index;
  details.mTexureBuffers = PDyMetaFontInformation::DBuiltin::TTexIndexes{IDB_PNG2};
}

} /// ::dy::builtin namespace