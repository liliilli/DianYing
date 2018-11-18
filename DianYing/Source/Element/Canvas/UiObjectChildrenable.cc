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
#include <Dy/Element/Canvas/UiObjectChildrenable.h>

namespace dy
{

void FDyUiObjectChildrenable::Render()
{
  for (auto& uiObject : this->mUiObjectList)
  {
    if (MDY_CHECK_ISEMPTY(uiObject) == true) { continue; }
    /// @TODO IMPLEMENT uiObject Is enabled or disabled.

    uiObject->Render();
  }
}

FDyUiObject* FDyUiObjectChildrenable::GetUiObject(const std::string& objectName, EDySearchMode searchMode)
{
  if (searchMode == EDySearchMode::Default)
  {
    for (std::unique_ptr<FDyUiObject>& objectPtr : this->mUiObjectList)
    {
      if (MDY_CHECK_ISEMPTY(objectPtr)) { continue; }
      if (objectPtr->GetUiObjectName() == objectName) { return objectPtr.get(); }
    }

    // FAILED
    return nullptr;
  }
  else
  {
    MDY_NOT_IMPLEMENTED_ASSERT();
  }
}

} /// ::dy namespace
