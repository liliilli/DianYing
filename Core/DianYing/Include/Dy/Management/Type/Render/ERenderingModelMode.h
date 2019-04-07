#ifndef GUARD_DY_MANAGEMENT_TYPE_RENDER_EDyModelRenderingMode_H
#define GUARD_DY_MANAGEMENT_TYPE_RENDER_EDyModelRenderingMode_H

namespace dy
{

/// @enum ERenderingModelMode
/// @brief Render model mesh with given mode.
enum class ERenderingModelMode : signed int
{
  FillNormal = 0, // Normal value. Model just rendered GL_FILL or etc which corresponds to others API.
  WireFrame  = 1,  // GL_LINE. Render wire frames.
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_TYPE_RENDER_EDyModelRenderingMode_H
