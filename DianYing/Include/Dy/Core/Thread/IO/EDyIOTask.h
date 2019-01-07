#ifndef GUARD_DY_CORE_THREAD_EDYIOTASK_H
#define GUARD_DY_CORE_THREAD_EDYIOTASK_H
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

#include <Dy/Helper/GlobalType.h>

namespace dy
{

///
/// @enum EDyResourceStyle
/// @brief Specifies each resource style (not type)
///
enum class EDyResourceStyle : TU08
{
  Information,
  Resource,
  NoneError
};

///
/// @enum EDyObject
/// @brief Object types for reflection, type checking and so on.
///
enum class EDyObject : TU08
{
  Object_Actor,
  UI_Font,
  UI_Image,
  Component_ModelFilter,
  Component_ModelRenderer,

  Etc_NotBindedYet,
  NoneError
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_THREAD_EDYIOTASK_H