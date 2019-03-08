#ifndef GAURD_DYFONTATLASGENERATOR_ACONSTANTLANGRANGE_H
#define GAURD_DYFONTATLASGENERATOR_ACONSTANTLANGRANGE_H
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

/// @brief Get range with [start, end].
/// This must be proceeded with compiled time.
constexpr auto GetRangeFrom(const int start, const int end) noexcept
{
  return end - start + 1;
}

static constexpr auto STANDARD_UNITPEREM = 256;
static constexpr auto TEXTURE_CANVAS_S   = 1024;
static constexpr auto TEXTURE_CANVAS_T   = 1024;
static constexpr auto TEXTURE_SIZE_S     = 64;
static constexpr auto TEXTURE_SIZE_T     = 64;
static constexpr auto CHANNEL_LIMIT      = 4;
static constexpr auto TEXTURE_MAPLIMIT   = (TEXTURE_CANVAS_S / TEXTURE_SIZE_S) * (TEXTURE_CANVAS_T / TEXTURE_SIZE_T) * CHANNEL_LIMIT;
static constexpr auto TEXTURE_PXRANGE    = 12.0;

constexpr auto ENGLISH_CHR_START = 0x0000;
constexpr auto ENGLISH_CHR_END   = 0x007F;
constexpr auto ENGLISH_CHR_RANGE = GetRangeFrom(ENGLISH_CHR_START, ENGLISH_CHR_END);

constexpr auto HANGUL_CHR_START = 0xAC00;
constexpr auto HANGUL_CHR_END   = 0xD7AF;
constexpr auto HANGUL_CHR_RANGE = GetRangeFrom(HANGUL_CHR_START, HANGUL_CHR_END);

constexpr auto KANA_CHR_START = 0x3000;
constexpr auto KANA_CHR_END   = 0x30FF;
constexpr auto KANA_CHR_RANGE = GetRangeFrom(KANA_CHR_START, KANA_CHR_END);

constexpr auto CJK_CHR_START = 0x4E00;
constexpr auto CJK_CHR_END   = 0x9FFF;
constexpr auto CJK_CHR_RANGE = GetRangeFrom(CJK_CHR_START, CJK_CHR_END);

} /// ::dy namespace

#endif /// GAURD_DYFONTATLASGENERATOR_ACONSTANTLANGRANGE_H