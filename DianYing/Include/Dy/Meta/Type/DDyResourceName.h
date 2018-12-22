#ifndef GUARD_DY_COMPONENT_ABSTRACT_ADYLOADINGBOOTSCRIPT_H
#define GUARD_DY_COMPONENT_ABSTRACT_ADYLOADINGBOOTSCRIPT_H
///
/// MIT License
/// Copyright (c) 2018 Jongmin Yun
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.
///

#include <Dy/Meta/Type/EDyResourceType.h>

namespace dy
{

/// @brief Resource Name for just only `GLOBAL` and `BOOTING`
/// `GLOBAL` is async but `BOOTING` is sync.
struct DDyResourceName final
{
  EDyResourceType mResourceType = EDyResourceType::NoneError;
  std::string     mName         = MDY_INITIALIZE_EMPTYSTR;

  DDyResourceName(_MIN_ EDyResourceType type, _MIN_ const std::string& name)
      : mResourceType{type}, mName{name} {};
  DDyResourceName(_MIN_ EDyResourceType type, _MIN_ const std::string_view& name)
      : mResourceType{type}, mName{MSVSTR(name)} {};
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_ABSTRACT_ADYLOADINGBOOTSCRIPT_H