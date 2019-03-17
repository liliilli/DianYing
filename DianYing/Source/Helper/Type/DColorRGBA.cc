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
#include <Dy/Helper/Type/DColorRGBA.h>
#include <Dy/Helper/Type/DColorRGBA32.h>

namespace dy {

const DColorRGBA DColorRGBA::Aqua       = DColorRGBA32{0x00, 0xAA, 0xFF};
const DColorRGBA DColorRGBA::Black      = DColorRGBA32{0x00, 0x00, 0x00};
const DColorRGBA DColorRGBA::Blue       = DColorRGBA32{0x00, 0x00, 0xFF};
const DColorRGBA DColorRGBA::DarkRed    = DColorRGBA32{0xAA, 0x00, 0x00};
const DColorRGBA DColorRGBA::DarkGray   = DColorRGBA32{0x55, 0x55, 0x55};
const DColorRGBA DColorRGBA::DarkGreen  = DColorRGBA32{0x00, 0xAA, 0x00};
const DColorRGBA DColorRGBA::Gold       = DColorRGBA32{0xFF, 0xAA, 0x00};
const DColorRGBA DColorRGBA::Gray       = DColorRGBA32{0xAA, 0xAA, 0xAA};
const DColorRGBA DColorRGBA::Green      = DColorRGBA32{0x00, 0xFF, 0x00};
const DColorRGBA DColorRGBA::Magenta    = DColorRGBA32{0xAA, 0x00, 0xAA};
const DColorRGBA DColorRGBA::Orange     = DColorRGBA32{0xFF, 0x55, 0x00};
const DColorRGBA DColorRGBA::Purple     = DColorRGBA32{0xAA, 0x00, 0xFF};
const DColorRGBA DColorRGBA::Red        = DColorRGBA32{0xFF, 0x00, 0x00};
const DColorRGBA DColorRGBA::White      = DColorRGBA32{0xFF, 0xFF, 0xFF};
const DColorRGBA DColorRGBA::Yellow     = DColorRGBA32{0xFF, 0xFF, 0x00};

DColorRGBA::DColorRGBA(TF32 r, TF32 g, TF32 b) noexcept 
  : R{r}, G{g}, B{b}, A{1.0f} 
{
  this->R = std::clamp(this->R, 0.f, 1.f);
  this->G = std::clamp(this->G, 0.f, 1.f);
  this->B = std::clamp(this->B, 0.f, 1.f);
}

DColorRGBA::DColorRGBA(TF32 r, TF32 g, TF32 b, TF32 a) noexcept 
  : R{r}, G{g}, B{b}, A{a} 
{
  this->R = std::clamp(this->R, 0.f, 1.f);
  this->G = std::clamp(this->G, 0.f, 1.f);
  this->B = std::clamp(this->B, 0.f, 1.f);
  this->A = std::clamp(this->A, 0.f, 1.f);
}

DColorRGBA::DColorRGBA(const std::array<TF32, 4>& iGlRgbaColor) noexcept
  : R{iGlRgbaColor[0]}, G{iGlRgbaColor[1]}, B{iGlRgbaColor[2]}, A{iGlRgbaColor[3]}
{ 
  this->R = std::clamp(this->R, 0.f, 1.f);
  this->G = std::clamp(this->G, 0.f, 1.f);
  this->B = std::clamp(this->B, 0.f, 1.f);
  this->A = std::clamp(this->A, 0.f, 1.f);
}

bool DColorRGBA::IsOpaque() const noexcept
{
  return this->A == 1.0f;
}

void DColorRGBA::SetOpaque() noexcept
{
  this->A = 1.0f;
}

TF32 DColorRGBA::GetGrayScale() const noexcept
{
  return 0.2126f * this->R + 0.7152f * this->G + 0.0722f * this->B;
}

const TF32* DColorRGBA::Data() const noexcept
{
  return &this->R;
}

std::array<TF32, 4> DColorRGBA::ToArray() const noexcept
{
  return {this->R, this->G, this->B, this->A};
}

DColorRGBA::operator DVector3() const noexcept
{
  return DVector3{this->R, this->G, this->B};
}

DColorRGBA::operator glm::vec3() const noexcept
{
  return glm::vec3{this->R, this->G, this->B};
}

DColorRGBA::operator glm::vec4() const noexcept
{
  return glm::vec4{this->R, this->G, this->B, this->A};
}

DColorRGBA::operator DVector4() const noexcept
{
  return DVector4{this->R, this->G, this->B, this->A};
}

void to_json(nlohmann::json& oJson, const DColorRGBA& iItem)
{
  oJson = nlohmann::json { {"R", iItem.R}, {"G", iItem.G}, {"B", iItem.B}, {"A", iItem.A} };
}

void from_json(const nlohmann::json& iJson, DColorRGBA& oItem)
{
  json::GetValueFromTo(iJson, "R", oItem.R);
  json::GetValueFromTo(iJson, "G", oItem.G);
  json::GetValueFromTo(iJson, "B", oItem.B);
  json::GetValueFromTo(iJson, "A", oItem.A);
}

} /// ::dy namespace
