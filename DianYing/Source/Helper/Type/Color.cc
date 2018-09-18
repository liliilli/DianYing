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
#include <Dy/Helper/Type/Color.h>
#include <Dy/Helper/Type/Color32.h>

namespace dy {

const DDyColor DDyColor::Aqua       = DDyColor32{0x00, 0xAA, 0xFF};
const DDyColor DDyColor::Black      = DDyColor32{0x00, 0x00, 0x00};
const DDyColor DDyColor::Blue       = DDyColor32{0x00, 0x00, 0xFF};
const DDyColor DDyColor::DarkRed    = DDyColor32{0xAA, 0x00, 0x00};
const DDyColor DDyColor::DarkGray   = DDyColor32{0x55, 0x55, 0x55};
const DDyColor DDyColor::DarkGreen  = DDyColor32{0x00, 0xAA, 0x00};
const DDyColor DDyColor::Gold       = DDyColor32{0xFF, 0xAA, 0x00};
const DDyColor DDyColor::Gray       = DDyColor32{0xAA, 0xAA, 0xAA};
const DDyColor DDyColor::Green      = DDyColor32{0x00, 0xFF, 0x00};
const DDyColor DDyColor::Magenta    = DDyColor32{0xAA, 0x00, 0xAA};
const DDyColor DDyColor::Orange     = DDyColor32{0xFF, 0x55, 0x00};
const DDyColor DDyColor::Purple     = DDyColor32{0xAA, 0x00, 0xFF};
const DDyColor DDyColor::Red        = DDyColor32{0xFF, 0x00, 0x00};
const DDyColor DDyColor::White      = DDyColor32{0xFF, 0xFF, 0xFF};
const DDyColor DDyColor::Yellow     = DDyColor32{0xFF, 0xFF, 0x00};

} /// ::dy namespace
