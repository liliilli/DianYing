#include <precompiled.h>
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

/// Header file
#include <Dy/Core/Resource/Information/FDySoundInformation.h>
#include <Dy/Meta/Information/MetaInfoSound.h>

namespace dy
{

FDySoundInformation::FDySoundInformation(_MIN_ const PDySoundInstanceMetaInfo& iInput) :
  mSpecifier{iInput.mSpecifierName},
  mPath{iInput.mExternalPath}
{ }

const std::string& FDySoundInformation::GetSpecifierName() const noexcept
{
  return this->mSpecifier;
}

const std::filesystem::path& FDySoundInformation::GetPath() const noexcept
{
  return this->mPath;
}

} /// ::dy namespace