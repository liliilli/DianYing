#ifndef GUARD_DY_COMPONENT_ABSTRACT_ADYLOADINGBOOTSCRIPT_H
#define GUARD_DY_COMPONENT_ABSTRACT_ADYLOADINGBOOTSCRIPT_H
///
/// MIT License
/// Copyright (c) 2018-2019 Jongmin Yun
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.
///

#include <Dy/Meta/Type/EResourceType.h>
#include <unordered_set>

namespace dy
{

/// @brief Resource Name for just only `GLOBAL` and `BOOTING`
/// `GLOBAL` is async but `BOOTING` is sync.
struct DDyResourceName final
{
  EResourceType mResourceType = EResourceType::NoneError;
  std::string     mName         = MDY_INITIALIZE_EMPTYSTR;

  DDyResourceName(_MIN_ EResourceType type, _MIN_ const std::string& name)
      : mResourceType{type}, mName{name} {};
};

/// @brief operator== overloading for `unordered_set` container.
inline bool operator==(_MIN_ const DDyResourceName& lhs, _MIN_ const DDyResourceName& rhs) noexcept 
{
    return lhs.mName == rhs.mName && lhs.mResourceType == rhs.mResourceType;
}

/// @struct Hash_DDyResourceName
/// @brief Hash functor.
struct Hash_DDyResourceName final
{
  std::size_t operator()(DDyResourceName const& s) const noexcept
  {
    return std::hash<std::string>{}(
        fmt::format("{}{}", 
            static_cast<std::underlying_type_t<decltype(s.mResourceType)>>(s.mResourceType), 
            s.mName
        )
    );
  }
};

using TDDyResourceNameSet = std::unordered_set<DDyResourceName, Hash_DDyResourceName>;

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_ABSTRACT_ADYLOADINGBOOTSCRIPT_H