#ifndef GUARD_DY_HELPER_COORDINATEBOUNDS_H
#define GUARD_DY_HELPER_COORDINATEBOUNDS_H
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

#include <nlohmann/json.hpp>
#include <Include/Type/Vector2.h>

namespace dy
{

/// @struct DDyCoordinateBounds
/// @brief
struct DDyCoordinateBounds final
{
  DDyVector2 mLeftDown{};
  DDyVector2 mRightUp {};
  uint8_t    mMapIndex{};
  uint8_t    mChannel {};
};

inline void to_json(nlohmann::json& j, const DDyCoordinateBounds& p)
{
  j = nlohmann::json
  {
      {"LeftDown", p.mLeftDown},
      {"RightUp",  p.mRightUp},
      {"MapIndex", p.mMapIndex},
      {"Channel",  p.mChannel}
  };
}

inline void from_json(const nlohmann::json& j, DDyCoordinateBounds& p)
{
  p.mLeftDown = j.at("LeftDown").get<DDyVector2>();
  p.mRightUp  = j.at("RightUp") .get<DDyVector2>();
  p.mMapIndex = j.at("MapIndex").get<uint8_t>();
  p.mChannel  = j.at("Channel").get<uint8_t>();
}

/// @brief
/// @param canvasWidth
/// @param canvasHeight
/// @param textureWidth
/// @param textureHeight
/// @param id
/// @return
DDyCoordinateBounds CreateCoordinateInformation(
    const int32_t canvasWidth,
    const int32_t canvasHeight,
    const int32_t textureWidth,
    const int32_t textureHeight,
    const int32_t id);

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_COORDINATEBOUNDS_H