#ifndef GUARD_DY_MANAGEMENT_TYPE_FONTRESOURCECONTAINER_H
#define GUARD_DY_MANAGEMENT_TYPE_FONTRESOURCECONTAINER_H
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

#include <Dy/Meta/Information/FontMetaInformation.h>

namespace dy
{

///
/// @class FDyFontResourceContainer
/// @brief
///
class FDyFontResourceContainer final
{
public:
  ///
  /// @brief This constructor must success.
  /// @param fontInformation
  ///
  FDyFontResourceContainer(_MIN_ const PDyMetaFontInformation& fontInformation);
  ~FDyFontResourceContainer();

private:

};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_TYPE_FONTRESOURCECONTAINER_H