#include <precompiled.h>
#if defined(MDY_FLAG_IN_EDITOR)
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
#include <Dy/Editor/Gui/ViewDetailView.h>

namespace dy::editor
{

EDySuccess FDyViewDetailView::pfInitialize(const PDyGuiComponentEmptyDescriptor& desc)
{
  return EDySuccess::DY_SUCCESS;
}

EDySuccess FDyViewDetailView::pfRelease()
{
  return EDySuccess::DY_SUCCESS;
}

void FDyViewDetailView::DrawWindow(float dt) noexcept
{

}

void FDyViewDetailView::Update(float dt) noexcept
{

}

} /// ::dy::editor namespace

#endif /// MDY_FLAG_IN_EDITOR