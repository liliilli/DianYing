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
#include <Dy/Helper/Type/DArea2D.h>
#include <Dy/Management/Type/AttachmentInformation.h>

namespace dy
{

/// @struct DGlGlobalStates
/// @brief Global states type for managing OpenGL internal rendering library safely.
struct DGlGlobalStates final
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

  /// @struct DCullfaceMode
  /// @brief Cullface mode descriptor.
  struct DCullfaceMode final
  {
    enum class EValue { Front, Back, FrontAndBack };

    EValue mValue = EValue::Front;
    DCullfaceMode(EValue iValue) : mValue{iValue} {};
  };
  
  /// @struct DViewport
  /// @brief Viewport mode descriptor.
  struct DViewport final
  {
    /// @struct DItem
    struct DItem final
    {
      TI32 mIndex;
      DArea2D mArea;

      DItem(TI32 iIndex, DArea2D iArea) : mIndex{iIndex}, mArea{iArea} {};
    };

    std::vector<DItem> mViewportSettingList;
  };

  std::optional<bool> mIsEnableBlend;
  std::optional<bool> mIsEnableCullface;
  std::optional<bool> mIsEnableDepthTest;
  std::optional<bool> mIsEnableScissorTest;

  std::optional<DPolygonMode>   mPolygonMode;
  std::optional<DBlendMode>     mBlendMode;
  std::optional<DCullfaceMode>  mCullfaceMode;
  std::optional<DViewport>      mViewportSettingList;

  std::optional<std::vector<PBlendingEquation>> mAttachmentBlendings;
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_TYPE_RENDER_DDyGLGlobalStatus_H