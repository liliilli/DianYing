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
#include <Dy/Helper/Type/ColorRGBA32.h>

namespace dy {

const DDyColorRGBA32 DDyColorRGBA32::Aqua = DDyColorRGBA32{0x00, 0xAA, 0xFF};
const DDyColorRGBA32 DDyColorRGBA32::Black = DDyColorRGBA32{0x00, 0x00, 0x00};
const DDyColorRGBA32 DDyColorRGBA32::Blue = DDyColorRGBA32{0x00, 0x00, 0xFF};
const DDyColorRGBA32 DDyColorRGBA32::DarkRed = DDyColorRGBA32{0xAA, 0x00, 0x00};
const DDyColorRGBA32 DDyColorRGBA32::DarkGray = DDyColorRGBA32{0x55, 0x55, 0x55};
const DDyColorRGBA32 DDyColorRGBA32::DarkGreen = DDyColorRGBA32{0x00, 0xAA, 0x00};
const DDyColorRGBA32 DDyColorRGBA32::Gold = DDyColorRGBA32{0xFF, 0xAA, 0x00};
const DDyColorRGBA32 DDyColorRGBA32::Gray = DDyColorRGBA32{0xAA, 0xAA, 0xAA};
const DDyColorRGBA32 DDyColorRGBA32::Green = DDyColorRGBA32{0x00, 0xFF, 0x00};
const DDyColorRGBA32 DDyColorRGBA32::Magenta = DDyColorRGBA32{0xAA, 0x00, 0xAA};
const DDyColorRGBA32 DDyColorRGBA32::Orange = DDyColorRGBA32{0xFF, 0x55, 0x00};
const DDyColorRGBA32 DDyColorRGBA32::Purple = DDyColorRGBA32{0xAA, 0x00, 0xFF};
const DDyColorRGBA32 DDyColorRGBA32::Red = DDyColorRGBA32{0xFF, 0x00, 0x00};
const DDyColorRGBA32 DDyColorRGBA32::White = DDyColorRGBA32{0xFF, 0xFF, 0xFF};
const DDyColorRGBA32 DDyColorRGBA32::Yellow = DDyColorRGBA32{0xFF, 0xFF, 0x00};

} /// ::dy namespace