#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Helper/Type/Color32.cc
///
/// @author Jongmin Yun
///
/// @log
/// 2018-07-06 Set up static const built-in colors.
///

/// Header file
#include <Dy/Helper/Type/color32.h>

namespace dy {

const DColor32 DColor32::Aqua = DColor32{0x00, 0xAA, 0xFF};
const DColor32 DColor32::Black = DColor32{0x00, 0x00, 0x00};
const DColor32 DColor32::Blue = DColor32{0x00, 0x00, 0xFF};
const DColor32 DColor32::DarkRed = DColor32{0xAA, 0x00, 0x00};
const DColor32 DColor32::DarkGray = DColor32{0x55, 0x55, 0x55};
const DColor32 DColor32::DarkGreen = DColor32{0x00, 0xAA, 0x00};
const DColor32 DColor32::Gold = DColor32{0xFF, 0xAA, 0x00};
const DColor32 DColor32::Gray = DColor32{0xAA, 0xAA, 0xAA};
const DColor32 DColor32::Green = DColor32{0x00, 0xFF, 0x00};
const DColor32 DColor32::Magenta = DColor32{0xAA, 0x00, 0xAA};
const DColor32 DColor32::Orange = DColor32{0xFF, 0x55, 0x00};
const DColor32 DColor32::Purple = DColor32{0xAA, 0x00, 0xFF};
const DColor32 DColor32::Red = DColor32{0xFF, 0x00, 0x00};
const DColor32 DColor32::White = DColor32{0xFF, 0xFF, 0xFF};
const DColor32 DColor32::Yellow = DColor32{0xFF, 0xFF, 0x00};

} /// ::dy namespace