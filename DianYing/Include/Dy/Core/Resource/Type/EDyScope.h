#ifndef GUARD_DY_CORE_RESOURCES_TYPE_EDYSCOPE_H
#define GUARD_DY_CORE_RESOURCES_TYPE_EDYSCOPE_H
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

namespace dy
{

///
/// @enum   EDyScope
/// @brief  Specify scope for resources (information and resource).
///
enum class EDyScope
{
  Global,
  Scene,
  Temporal,
  UserDefined,
};

MDY_NODISCARD inline bool operator>(_MIN_ EDyScope lhs, _MIN_ EDyScope rhs) noexcept
{
  switch (lhs)
  {
  case EDyScope::Global:
  {
    if (rhs != EDyScope::Global)  { return true; }
    else                          { return false; }
  }
  case EDyScope::Scene:
  {
    switch (rhs)
    {
    case EDyScope::Global:
    case EDyScope::Scene:       return false;
    case EDyScope::Temporal:    return true;
    case EDyScope::UserDefined: return false;
    }
  }
  default: return false;
  }
}

MDY_NODISCARD inline bool operator>=(_MIN_ EDyScope lhs, _MIN_ EDyScope rhs) noexcept
{
  switch (lhs)
  {
  case EDyScope::Global: { return true; }
  case EDyScope::Scene:
  {
    switch (rhs)
    {
    case EDyScope::Scene:
    case EDyScope::Temporal: return true;
    case EDyScope::Global:
    case EDyScope::UserDefined: return false;
    }
  }
  default: return lhs == rhs;
  }
}

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCES_TYPE_EDYSCOPE_H