#ifndef GUARD_DY_MANAGEMENT_INLINE_MLOG_INL
#define GUARD_DY_MANAGEMENT_INLINE_MLOG_INL
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

template <typename... TArgs>
void DyPushLogInfo(const std::string& iFormatString, TArgs&&... iArgs)
{
  ::dy::MLog::GetInstance().PushLog(
      ::dy::MLog::ELevel::Information, 
      ::dy::MakeStringU8(iFormatString, std::forward<TArgs>(iArgs)...)
  );
}

template <typename... TArgs>
void DyPushLogWarning(const std::string& iFormatString, TArgs&&... iArgs)
{
  ::dy::MLog::GetInstance().PushLog(
      ::dy::MLog::ELevel::Warning, 
      ::dy::MakeStringU8(iFormatString, std::forward<TArgs>(iArgs)...)
  );
}

template <typename... TArgs>
void DyPushLogError(const std::string& iFormatString, TArgs&&... iArgs)
{
  ::dy::MLog::GetInstance().PushLog(
      ::dy::MLog::ELevel::Error, 
      ::dy::MakeStringU8(iFormatString, std::forward<TArgs>(iArgs)...)
  );
}

template <typename... TArgs>
void DyPushLogCritical(const std::string& iFormatString, TArgs&&... iArgs)
{
  ::dy::MLog::GetInstance().PushLog(
      ::dy::MLog::ELevel::Critical, 
      ::dy::MakeStringU8(iFormatString, std::forward<TArgs>(iArgs)...)
  );
}

#if (defined(NDEBUG) == false)
  template <typename... TArgs>
  void DyPushLogDebugTrace(const std::string& iFormatString, TArgs&&... iArgs)
  {
    ::dy::MLog::GetInstance().PushLog(
        ::dy::MLog::ELevel::Trace, 
        ::dy::MakeStringU8(iFormatString, std::forward<TArgs>(iArgs)...)
    );
  }

  template <typename... TArgs>
  void DyPushLogDebugDebug(const std::string& iFormatString, TArgs&&... iArgs)
  {
    ::dy::MLog::GetInstance().PushLog(
        ::dy::MLog::ELevel::Debug, 
        ::dy::MakeStringU8(iFormatString, std::forward<TArgs>(iArgs)...)
    );
  }

  template <typename... TArgs>
  void DyPushLogDebugInfo(const std::string& iFormatString, TArgs&&... iArgs)
  {
    ::dy::MLog::GetInstance().PushLog(
        ::dy::MLog::ELevel::Information, 
        ::dy::MakeStringU8(iFormatString, std::forward<TArgs>(iArgs)...)
    );
  }

  template <typename... TArgs>
  void DyPushLogDebugWarning(const std::string& iFormatString, TArgs&&... iArgs)
  {
    ::dy::MLog::GetInstance().PushLog(
        ::dy::MLog::ELevel::Warning, 
        ::dy::MakeStringU8(iFormatString, std::forward<TArgs>(iArgs)...)
    );
  }

  template <typename... TArgs>
  void DyPushLogDebugError(const std::string& iFormatString, TArgs&&... iArgs)
  {
    ::dy::MLog::GetInstance().PushLog(
        ::dy::MLog::ELevel::Error, 
        ::dy::MakeStringU8(iFormatString, std::forward<TArgs>(iArgs)...)
    );
  }

  template <typename... TArgs>
  void DyPushLogDebugCritical(const std::string& iFormatString, TArgs&&... iArgs)
  {
    ::dy::MLog::GetInstance().PushLog(
        ::dy::MLog::ELevel::Critical, 
        ::dy::MakeStringU8(iFormatString, std::forward<TArgs>(iArgs)...)
    );
  }

#else
  #define DyPushLogDebugTrace(__MAString__, ...)    ((void*)0)
  #define DyPushLogDebugInfo(__MAString__, ...)     ((void*)0)
  #define DyPushLogDebugDebug(__MAString__, ...)    ((void*)0)
  #define DyPushLogDebugWarning(__MAString__, ...)  ((void*)0)
  #define DyPushLogDebugCritical(__MAString__, ...) ((void*)0)
  #define DyPushLogDebugError(__MAString__, ...)    ((void*)0)
#endif /// opgs16::debug::PushLog only on _DEBUG

#endif /// GUARD_DY_MANAGEMENT_INLINE_MLOG_INL