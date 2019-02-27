#pragma once
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

#include <mutex>
#include <vector>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <Include/Type/DResult.h>
#include <Include/Library/MsdfgenHelper.h>
#include <Include/Type/DFileInformations.h>

namespace dy
{

class FTaskThread final
{
public:
  FTaskThread(const std::vector<DDyFontInformation>& informations);
  // We must release freetype fonts.
  ~FTaskThread();

  using TSdfType          = msdfgen::Bitmap<float>;
  using TCharMapRangePair = std::pair<uint64_t, uint64_t>;

  void operator()(std::vector<DResult>& charResultList, 
    const std::vector<FT_ULong>& charMapList, 
    const TCharMapRangePair charRangePair);

private:
  void AtomicInitializeFreetype(const std::vector<DDyFontInformation>& informations) noexcept;

  /// @struct DAlignedBBoxInfo
  /// @brief
  struct DAlignedBBoxInfo final
  {
    double            range{};
    msdfgen::Vector2  scale;
    msdfgen::Vector2  translate;
  };
    
  /// @struct DBound2D
  /// @brief
  struct DBound2D final
  {
    double l = std::numeric_limits<double>::max();
    double b = std::numeric_limits<double>::max();
    double r = std::numeric_limits<double>::lowest();
    double t = std::numeric_limits<double>::lowest();
  };

  /// @brief Checks Bounds boundary validity.
  void CheckAndChangeBounds(DBound2D& bounds)
  {
    if (bounds.l >= bounds.r || bounds.b >= bounds.t)
    {
      bounds.l = 0; bounds.b = 0; bounds.r = 1; bounds.t = 1;
    }
  };

  [[nodiscard]] DAlignedBBoxInfo AlignSDFBBoxFrame(const DBound2D& bounds);
  [[nodiscard]] QImage CreateQImageFromSDFBuffer(const TSdfType& buffer);

  inline static std::mutex sFtCommonMutex;
  std::vector<std::pair<FT_Library, FT_Face>> mLocalFtInstance;
  
  FT_Outline_Funcs ftFunctions;
};

} /// ::dy namespace
