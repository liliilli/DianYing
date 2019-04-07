#ifndef GUARD_DY_EDITOR_DESCRIPTOR_EMPTY_DESCRIPTOR_H
#define GUARD_DY_EDITOR_DESCRIPTOR_EMPTY_DESCRIPTOR_H
#if defined(MDY_FLAG_IN_EDITOR)

namespace dy::editor
{

///
/// @struct PDyGuiComponentEmptyDescriptor
/// @brief Empty descriptor for gui component construction.
///
struct PDyGuiComponentEmptyDescriptor final
{};

} /// ::dy::editor namespace

#endif /// MDY_FLAG_IN_EDITOR
#endif /// GUARD_DY_EDITOR_DESCRIPTOR_EMPTY_DESCRIPTOR_H