#ifndef GUARD_DY_MANAGEMENT_TYPE_RENDER_DDyGLGlobalStatus_H
#define GUARD_DY_MANAGEMENT_TYPE_RENDER_DDyGLGlobalStatus_H
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

#include <optional>

namespace dy
{

/// @struct DDyGlGlobalStatus
/// @brief Global status type for managing OpenGL internal rendering library safely.
struct DDyGlGlobalStatus final
{
  /// @struct DPolygonMode
  /// @brief Polygon mode descriptor.
  struct DPolygonMode final
  {
    enum class EMode { Front, Back, FrontAndBack, };
    enum class EValue { Triangle, Line, Point, };

    EMode   mMode   = EMode::FrontAndBack;
    EValue  mValue  = EValue::Triangle;

    DPolygonMode(EMode iMode, EValue iValue) : mMode{iMode}, mValue{iValue} {};
  };

  /// @struct DBlendMode
  /// @brief Blend mode descriptor.
  struct DBlendMode final
  {
    enum class EEqut { SrcAddDst, SrcSubDst, DstSubSrc, CompareMin, CompareMax };
    enum class EFunc { Zero, One, SrcColor, OneMinusSrcColor, SrcAlpha, OneMinusSrcAlpha };

    /// @struct DItem
    struct DItem
    {
      EEqut mEquation;
      EFunc mSourceFunc;
      EFunc mDestinationFunc;

      DItem(EEqut iEquation, EFunc iSrcFunc, EFunc iDstFunc) : 
          mEquation{iEquation}, mSourceFunc{iSrcFunc}, mDestinationFunc{iDstFunc} {};
    };

    std::vector<DItem> mBlendingSettingList;
  };

  std::optional<bool> mIsEnableBlend;
  std::optional<bool> mIsEnableCullface;
  std::optional<bool> mIsEnableDepthTest;
  std::optional<bool> mIsEnableScissorTest;
  std::optional<DPolygonMode> mPolygonMode;
  std::optional<DBlendMode>   mBlendMode;
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_TYPE_RENDER_DDyGLGlobalStatus_H