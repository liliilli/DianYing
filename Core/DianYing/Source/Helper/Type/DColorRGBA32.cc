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
#include <Dy/Helper/Type/DColorRGBA32.h>
#include <nlohmann/json.hpp>

namespace dy {

const DColorRGBA32 DColorRGBA32::Aqua = DColorRGBA32{0x00, 0xAA, 0xFF};
const DColorRGBA32 DColorRGBA32::Black = DColorRGBA32{0x00, 0x00, 0x00};
const DColorRGBA32 DColorRGBA32::Blue = DColorRGBA32{0x00, 0x00, 0xFF};
const DColorRGBA32 DColorRGBA32::DarkRed = DColorRGBA32{0xAA, 0x00, 0x00};
const DColorRGBA32 DColorRGBA32::DarkGray = DColorRGBA32{0x55, 0x55, 0x55};
const DColorRGBA32 DColorRGBA32::DarkGreen = DColorRGBA32{0x00, 0xAA, 0x00};
const DColorRGBA32 DColorRGBA32::Gold = DColorRGBA32{0xFF, 0xAA, 0x00};
const DColorRGBA32 DColorRGBA32::Gray = DColorRGBA32{0xAA, 0xAA, 0xAA};
const DColorRGBA32 DColorRGBA32::Green = DColorRGBA32{0x00, 0xFF, 0x00};
const DColorRGBA32 DColorRGBA32::Magenta = DColorRGBA32{0xAA, 0x00, 0xAA};
const DColorRGBA32 DColorRGBA32::Orange = DColorRGBA32{0xFF, 0x55, 0x00};
const DColorRGBA32 DColorRGBA32::Purple = DColorRGBA32{0xAA, 0x00, 0xFF};
const DColorRGBA32 DColorRGBA32::Red = DColorRGBA32{0xFF, 0x00, 0x00};
const DColorRGBA32 DColorRGBA32::White = DColorRGBA32{0xFF, 0xFF, 0xFF};
const DColorRGBA32 DColorRGBA32::Yellow = DColorRGBA32{0xFF, 0xFF, 0x00};

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const DColorRGBA32& p)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ DColorRGBA32& p)
{
  p.R = j["R"];
  p.G = j["G"];
  p.B = j["B"];
  p.A = j["A"];
}

} /// ::dy namespace