#ifndef GUARD_DY_BUILTIN_TEXTURE_ERRORBLUE_H
#define GUARD_DY_BUILTIN_TEXTURE_ERRORBLUE_H
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

#include <Dy/Builtin/Abstract/ATextureResource.h>

namespace dy::builtin
{

///
/// @class FDyBuiltinTextureErrorBlue
/// @brief Instantiate error blue builtin texture which set on if texture instantiation is failed.
///
class FDyBuiltinTextureErrorBlue final : public ATextureResource
{
  MDY_REGISTER_RESOURCE_TEXTURE(FDyBuiltinTextureErrorBlue, "dyBtTextureErrorBlue")
};

} /// ::dy::builtin namespace

#endif /// GUARD_DY_BUILTIN_TEXTURE_ERRORBLUE_H