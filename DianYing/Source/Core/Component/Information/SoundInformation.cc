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
#include <Dy/Core/Component/Information/SoundInformation.h>
#include <Dy/Management/LoggingManager.h>
#include <Dy/Core/Component/Resource/SoundResource.h>

namespace dy
{

DDySoundInformation::DDySoundInformation(const PDySoundConstructionDescriptor& soundDesc) :
    mDesc { soundDesc }
{ }

DDySoundInformation::~DDySoundInformation()
{
  if (this->__mLinkedSoundResourcePtr)
  {
    this->__mLinkedSoundResourcePtr->__pfResetSoundInformationLink();
  }
}

} /// ::dy namespace