#include <precompiled.h>
#if defined(MDY_FLAG_IN_EDITOR)
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
#include <Dy/Editor/Gui/LogWindow.h>

#include <imgui/imgui.h>
#include <Dy/Management/LoggingManager.h>
#include <Dy/Management/Helper/GuiLogStreamSink.h>

namespace dy::editor
{

EDySuccess FDyLogWindow::pfInitialize(const PDyGuiComponentEmptyDescriptor& desc)
{
  bool isFalse = false;
  while (!TGuiLogStreamSinkMt::sLogGuiActivated.compare_exchange_strong(isFalse, true));
  return DY_SUCCESS;
}

EDySuccess FDyLogWindow::pfRelease()
{
  bool isTrue = true;
  while (!TGuiLogStreamSinkMt::sLogGuiActivated.compare_exchange_strong(isTrue, false));
  {
    std::lock_guard<std::mutex> lck(TGuiLogStreamSinkMt::mLogMutex);
    while (!TGuiLogStreamSinkMt::mLogChunk.empty()) { TGuiLogStreamSinkMt::mLogChunk.pop(); }
  }

  MDY_LOG_INFO("FDyLogWindow::pfRelease");
  return DY_SUCCESS;
}

void FDyLogWindow::DrawWindow(float dt) noexcept
{
  this->pPushLog();

  ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
  ImGui::Begin("Log Window");
  if (ImGui::Button("Clear")) { pClearLog(); }

  ImGui::SameLine(); const bool copy = ImGui::Button("Copy");
  ImGui::SameLine(); this->mFilter.Draw("Filter", -100.0f);

  ImGui::Separator();

  ImGui::BeginChild("Scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
  if (copy) ImGui::LogToClipboard();

  if (this->mFilter.IsActive())
  {
    const TC8* buf_begin = this->mBuf.begin();
    const TC8* line = buf_begin;
    for (TI32 lineNumber = 0; line != nullptr; lineNumber++)
    {
      const TC8* lineEnd = lineNumber < this->mLineOffsets.Size ? buf_begin + this->mLineOffsets[lineNumber] : nullptr;
      if (this->mFilter.PassFilter(line, lineEnd))
      {
        ImGui::TextUnformatted(line, lineEnd);
      }
      line = lineEnd && lineEnd[1] ? lineEnd + 1 : nullptr;
    }
  }
  else
  {
    ImGui::TextUnformatted(this->mBuf.begin());
  }

  if (this->mScrollToBottom)
  {
    ImGui::SetScrollHere(1.0f);
  }
  this->mScrollToBottom = false;

  ImGui::EndChild();
  ImGui::End();
}

void FDyLogWindow::pClearLog() noexcept
{
  this->mBuf.clear();
  this->mLineOffsets.clear();
}

void FDyLogWindow::pPushLog() noexcept
{
  std::lock_guard<std::mutex> lck(TGuiLogStreamSinkMt::mLogMutex);
  while (!TGuiLogStreamSinkMt::mLogChunk.empty())
  {
    this->mBuf.appendf(TGuiLogStreamSinkMt::mLogChunk.front().c_str());
    TGuiLogStreamSinkMt::mLogChunk.pop();
  }
}

} /// ::dy::editor namespace
#endif