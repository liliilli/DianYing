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
#include <Dy/Core/Resource/Information_Deprecated/SoundInformation_Deprecated.h>
#include <Dy/Management/LoggingManager.h>
#include <Dy/Core/Resource/Resource_Deprecated/SoundResource_Deprecated.h>

namespace dy
{

DDySoundInformation_Deprecated::DDySoundInformation_Deprecated(const PDySoundConstructionDescriptor& soundDesc) :
    mDesc { soundDesc }
{ }

DDySoundInformation_Deprecated::~DDySoundInformation_Deprecated()
{
  if (this->__mLinkedSoundResourcePtr)
  {
    this->__mLinkedSoundResourcePtr->__pfResetSoundInformationLink();
  }
}

} /// ::dy namespace