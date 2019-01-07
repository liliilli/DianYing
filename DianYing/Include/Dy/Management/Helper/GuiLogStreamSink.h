#ifndef GUARD_DY_MANAGEMENT_HELPER_GUILOGSTREAMSINK_H
#define GUARD_DY_MANAGEMENT_HELPER_GUILOGSTREAMSINK_H
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

#include <spdlog/sinks/base_sink.h>
#include <sstream>
#include <queue>

namespace dy
{

///
/// @class FDyGuiLogStreamSink
/// @brief Spdlog logging sink. strongly binded to FDyLogWindow.
///
template <typename TMutex>
class FDyGuiLogStreamSink final : public spdlog::sinks::base_sink<TMutex>
{
protected:
  void sink_it_(const spdlog::details::log_msg& logMsg) override final
  {
    fmt::memory_buffer formatted;
    this->formatter_->format(logMsg, formatted);

    if (sLogGuiActivated.load(std::memory_order_seq_cst))
    {
      std::lock_guard<std::mutex> lck(mLogMutex);
      mLogChunk.push(fmt::to_string(formatted));
    }
  }

  void flush_() override final
  {
    std::stringstream flushedLogStream;
    flushedLogStream << std::flush;

    if (sLogGuiActivated.load(std::memory_order_seq_cst))
    {
      std::lock_guard<std::mutex> lck(mLogMutex);
      for (std::string line; std::getline(flushedLogStream, line);) { mLogChunk.push(line); }
    }
  }

public:
  static std::mutex               mLogMutex;
  static std::queue<std::string>  mLogChunk;
  static std::atomic<bool>        sLogGuiActivated;
};

} /// ::dy namespace

#include <spdlog/details/null_mutex.h>
#include <mutex>
using TGuiLogStreamSinkMt = dy::FDyGuiLogStreamSink<std::mutex>;

#endif /// GUARD_DY_MANAGEMENT_HELPER_GUILOGSTREAMSINK_H