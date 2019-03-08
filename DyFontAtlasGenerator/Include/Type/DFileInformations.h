#ifndef GUARD_TYPE_DFILEINFORMATIONS_H
#define GUARD_TYPE_DFILEINFORMATIONS_H
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

#include <string>
#include <set>

namespace dy
{

struct DDyFontInformation final
{
  /// File path on focus.
  std::string fontPath  = "";
  /// File name on focus.
  std::string fontName  = "";
  /// Font style on focus.
  std::string fontStyle = "";
  /// Font line feed height (64ths of pixel)
  int32_t     mLineFeedHeight = 0;
  /// Font line scaled height (also measured 64th of pixel)
  int32_t     mScaledLineFeedHeight = 0;
};

struct DDyTextFileInformation final
{
  std::set<uint16_t> mCharGlyphs;
};

} /// ::dy namespace

#endif /// GUARD_TYPE_DFILEINFORMATIONS_H