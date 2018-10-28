#ifndef GUARD_DY_BUILTIN_MODEL_SCREENPROJECTIONTRIANGLE_H
#define GUARD_DY_BUILTIN_MODEL_SCREENPROJECTIONTRIANGLE_H
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

#include <string_view>

namespace dy::builtin
{

///
/// @class FDyBuiltinModelScreenProjectionTriangle
/// @brief Used rendering entire screen once more with only 3 vertices
///
class FDyBuiltinModelScreenProjectionTriangle final
{
public:
  FDyBuiltinModelScreenProjectionTriangle();

  /// name for creating DDyModelInformation
  inline static MDY_SET_IMMUTABLE_STRING(sName, "dyBtModelScrProjTri");
};

} /// ::dy::builtin namespace

#endif /// GUARD_DY_BUILTIN_MODEL_SCREENPROJECTIONTRIANGLE_H