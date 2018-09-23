#ifndef GUARD_DY_EDITOR_GUI_DETAIL_TRANSFORM_H
#define GUARD_DY_EDITOR_GUI_DETAIL_TRANSFORM_H
#if defined(MDY_FLAG_IN_EDITOR)

#include <Dy/Editor/Interface/IDyGuiComponentBase.h>

namespace dy::editor
{

///
/// @class FDyDetailTransform
/// @brief
///
class FDyDetailTransform final : public IDyGuiComponentBase
{
  ///
  void Update(float dt) noexcept override final;
  ///
  void DrawWindow(float dt) noexcept override final;
};

} /// ::dy::editor namespace

#endif /// MDY_FLAG_IN_EDITOR
#endif /// GUARD_DY_EDITOR_GUI_DETAIL_TRANSFORM_H