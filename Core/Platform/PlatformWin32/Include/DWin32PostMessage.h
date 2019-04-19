#pragma once
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

#include <any>

namespace dy
{

/// @enum EWin32PostMesasge
/// @brief Win32 Post message type.
enum class EWin32PostMesasge
{
  DeleteWindow,
};

/// @struct DWin32PostMessage
/// @brief Win32 dependent Dy post message type.
struct DWin32PostMessage final
{
  EWin32PostMesasge mType = EWin32PostMesasge::DeleteWindow;
  std::any mData = nullptr;
};

} /// ::dy namespace