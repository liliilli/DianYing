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

namespace dy::builtin
{

void FBtFtDefaultGothic::ConstructBuffer(PDyMetaFontInformation::DBuiltin& oInfo) noexcept
{
  oInfo.mFontType = decltype(oInfo.mFontType)::SDF;
  oInfo.mFontInformation = 
#include "Gothic.info"
    ;

  oInfo.mFontTextureBuffers.resize(1);
  oInfo.mFontTextureBuffers.front() = {
//#include "Gothic.buffer"
  };
}

} /// ::dy::builtin namespace