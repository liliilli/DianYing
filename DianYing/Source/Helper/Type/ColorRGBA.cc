#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Helper/Type/Color.cc
/// @author Jongmin Yun
///
/// @log
/// 2018-08-28 Set up static const built-in colors.
///

/// Header file
#include <Dy/Helper/Type/ColorRGBA.h>
#include <Dy/Helper/Type/ColorRGBA32.h>

namespace dy {

const DDyColorRGBA DDyColorRGBA::Aqua       = DDyColorRGBA32{0x00, 0xAA, 0xFF};
const DDyColorRGBA DDyColorRGBA::Black      = DDyColorRGBA32{0x00, 0x00, 0x00};
const DDyColorRGBA DDyColorRGBA::Blue       = DDyColorRGBA32{0x00, 0x00, 0xFF};
const DDyColorRGBA DDyColorRGBA::DarkRed    = DDyColorRGBA32{0xAA, 0x00, 0x00};
const DDyColorRGBA DDyColorRGBA::DarkGray   = DDyColorRGBA32{0x55, 0x55, 0x55};
const DDyColorRGBA DDyColorRGBA::DarkGreen  = DDyColorRGBA32{0x00, 0xAA, 0x00};
const DDyColorRGBA DDyColorRGBA::Gold       = DDyColorRGBA32{0xFF, 0xAA, 0x00};
const DDyColorRGBA DDyColorRGBA::Gray       = DDyColorRGBA32{0xAA, 0xAA, 0xAA};
const DDyColorRGBA DDyColorRGBA::Green      = DDyColorRGBA32{0x00, 0xFF, 0x00};
const DDyColorRGBA DDyColorRGBA::Magenta    = DDyColorRGBA32{0xAA, 0x00, 0xAA};
const DDyColorRGBA DDyColorRGBA::Orange     = DDyColorRGBA32{0xFF, 0x55, 0x00};
const DDyColorRGBA DDyColorRGBA::Purple     = DDyColorRGBA32{0xAA, 0x00, 0xFF};
const DDyColorRGBA DDyColorRGBA::Red        = DDyColorRGBA32{0xFF, 0x00, 0x00};
const DDyColorRGBA DDyColorRGBA::White      = DDyColorRGBA32{0xFF, 0xFF, 0xFF};
const DDyColorRGBA DDyColorRGBA::Yellow     = DDyColorRGBA32{0xFF, 0xFF, 0x00};

} /// ::dy namespace
