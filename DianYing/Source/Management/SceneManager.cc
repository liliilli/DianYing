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
#include <Dy/Management/SceneManager.h>
#include <Dy/Management/LoggingManager.h>

namespace dy
{

EDySuccess MDyScene::pfInitialize()
{
  MDY_LOG_INFO_D("{} | MDyScene::pfInitialize().", "FunctionCall");
  return DY_SUCCESS;
}

EDySuccess MDyScene::pfRelease()
{
  MDY_LOG_INFO_D("{} | MDyScene::pfRelease().", "FunctionCall");
  return DY_SUCCESS;
}

const CDyCamera* MDyScene::GetCamera() const noexcept
{
  assert(this->mValidMainCameraPtr);
  return this->mValidMainCameraPtr;
}

void MDyScene::__pfBindFocusCamera(CDyCamera* validCameraPtr)
{
  assert(validCameraPtr);
  this->mValidMainCameraPtr = validCameraPtr;
}

void MDyScene::__pfUnbindCameraFocus()
{
  if (this->mValidMainCameraPtr)
  {
    this->mValidMainCameraPtr = nullptr;
    MDY_LOG_INFO_D("{} | MainCamera pointing unbinded.",
                   "MDyScene::__pfUnbindCameraFocus()");
  }
  else
  {
    MDY_LOG_WARNING_D("{} | Valid mainCamera pointer does not point anything.",
                      "MDyScene::__pfUnbindCameraFocus()");
  }
}

} /// ::dy namespace