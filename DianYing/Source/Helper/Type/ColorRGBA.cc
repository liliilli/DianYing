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

DDyColorRGBA::DDyColorRGBA(TF32 r, TF32 g, TF32 b) noexcept 
  : R{r}, G{g}, B{b}, A{1.0f} 
{
  this->R = std::clamp(this->R, 0.f, 1.f);
  this->G = std::clamp(this->G, 0.f, 1.f);
  this->B = std::clamp(this->B, 0.f, 1.f);
}

DDyColorRGBA::DDyColorRGBA(TF32 r, TF32 g, TF32 b, TF32 a) noexcept 
  : R{r}, G{g}, B{b}, A{a} 
{
  this->R = std::clamp(this->R, 0.f, 1.f);
  this->G = std::clamp(this->G, 0.f, 1.f);
  this->B = std::clamp(this->B, 0.f, 1.f);
  this->A = std::clamp(this->A, 0.f, 1.f);
}

DDyColorRGBA::DDyColorRGBA(const std::array<TF32, 4>& iGlRgbaColor) noexcept
  : R{iGlRgbaColor[0]}, G{iGlRgbaColor[1]}, B{iGlRgbaColor[2]}, A{iGlRgbaColor[3]}
{ 
  this->R = std::clamp(this->R, 0.f, 1.f);
  this->G = std::clamp(this->G, 0.f, 1.f);
  this->B = std::clamp(this->B, 0.f, 1.f);
  this->A = std::clamp(this->A, 0.f, 1.f);
}

bool DDyColorRGBA::IsOpaque() const noexcept
{
  return this->A == 1.0f;
}

void DDyColorRGBA::SetOpaque() noexcept
{
  this->A = 1.0f;
}

TF32 DDyColorRGBA::GetGrayScale() const noexcept
{
  return 0.2126f * this->R + 0.7152f * this->G + 0.0722f * this->B;
}

const TF32* DDyColorRGBA::Data() const noexcept
{
  return &this->R;
}

std::array<TF32, 4> DDyColorRGBA::ToArray() const noexcept
{
  return {this->R, this->G, this->B, this->A};
}

DDyColorRGBA::operator DDyVector3() const noexcept
{
  return DDyVector3{this->R, this->G, this->B};
}

DDyColorRGBA::operator glm::vec3() const noexcept
{
  return glm::vec3{this->R, this->G, this->B};
}

DDyColorRGBA::operator glm::vec4() const noexcept
{
  return glm::vec4{this->R, this->G, this->B, this->A};
}

DDyColorRGBA::operator DDyVector4() const noexcept
{
  return DDyVector4{this->R, this->G, this->B, this->A};
}

void to_json(nlohmann::json& oJson, const DDyColorRGBA& iItem)
{
  oJson = nlohmann::json { {"R", iItem.R}, {"G", iItem.G}, {"B", iItem.B}, {"A", iItem.A} };
}

void from_json(const nlohmann::json& iJson, DDyColorRGBA& oItem)
{
  DyJsonGetValueFromTo(iJson, "R", oItem.R);
  DyJsonGetValueFromTo(iJson, "G", oItem.G);
  DyJsonGetValueFromTo(iJson, "B", oItem.B);
  DyJsonGetValueFromTo(iJson, "A", oItem.A);
}

} /// ::dy namespace
