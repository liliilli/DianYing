#ifndef GUARD_DY_CORE_RESOURCES_TYPE_EDYSCOPE_H
#define GUARD_DY_CORE_RESOURCES_TYPE_EDYSCOPE_H
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

namespace dy
{

/// @enum   EResourceScope
/// @brief  Specify scope for resources (information and resource).
enum class EResourceScope
{
  Global,
  Level,
  Temporal,
  UserDefined,
};

MDY_NODISCARD inline bool operator>(EResourceScope lhs, EResourceScope rhs) noexcept
{
  switch (lhs)
  {
  case EResourceScope::Global:
  {
    if (rhs != EResourceScope::Global)  
    { return true; }
    else                          
    { return false; }
  }
  case EResourceScope::Level:
  {
    switch (rhs)
    {
    case EResourceScope::Global:
    case EResourceScope::Level:       return false;
    case EResourceScope::Temporal:    return true;
    case EResourceScope::UserDefined: return false;
    }
  }
  default: return false;
  }
}

MDY_NODISCARD inline bool operator>=(EResourceScope lhs, EResourceScope rhs) noexcept
{
  switch (lhs)
  {
  case EResourceScope::Global: { return true; }
  case EResourceScope::Level:
  {
    switch (rhs)
    {
    case EResourceScope::Level:
    case EResourceScope::Temporal: return true;
    case EResourceScope::Global:
    case EResourceScope::UserDefined: return false;
    }
  }
  default: return lhs == rhs;
  }
}

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCES_TYPE_EDYSCOPE_H