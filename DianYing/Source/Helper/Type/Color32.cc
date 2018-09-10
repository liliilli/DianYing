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

const DDyColor32 DDyColor32::Aqua = DDyColor32{0x00, 0xAA, 0xFF};
const DDyColor32 DDyColor32::Black = DDyColor32{0x00, 0x00, 0x00};
const DDyColor32 DDyColor32::Blue = DDyColor32{0x00, 0x00, 0xFF};
const DDyColor32 DDyColor32::DarkRed = DDyColor32{0xAA, 0x00, 0x00};
const DDyColor32 DDyColor32::DarkGray = DDyColor32{0x55, 0x55, 0x55};
const DDyColor32 DDyColor32::DarkGreen = DDyColor32{0x00, 0xAA, 0x00};
const DDyColor32 DDyColor32::Gold = DDyColor32{0xFF, 0xAA, 0x00};
const DDyColor32 DDyColor32::Gray = DDyColor32{0xAA, 0xAA, 0xAA};
const DDyColor32 DDyColor32::Green = DDyColor32{0x00, 0xFF, 0x00};
const DDyColor32 DDyColor32::Magenta = DDyColor32{0xAA, 0x00, 0xAA};
const DDyColor32 DDyColor32::Orange = DDyColor32{0xFF, 0x55, 0x00};
const DDyColor32 DDyColor32::Purple = DDyColor32{0xAA, 0x00, 0xFF};
const DDyColor32 DDyColor32::Red = DDyColor32{0xFF, 0x00, 0x00};
const DDyColor32 DDyColor32::White = DDyColor32{0xFF, 0xFF, 0xFF};
const DDyColor32 DDyColor32::Yellow = DDyColor32{0xFF, 0xFF, 0x00};

} /// ::dy namespace