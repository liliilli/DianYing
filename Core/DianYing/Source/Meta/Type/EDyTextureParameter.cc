#include <precompiled.h>
///
/// MIT License
/// Copyright (c) 2018-2019 Jongmin Yun
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.
///

/// Header file
#include <Dy/Meta/Type/EDyTextureParameter.h>
#include <nlohmann/json.hpp>

//!
//! Forward declaration
//!

namespace
{

template <TI32 TCount>
using TTargetList = std::array<dy::EDyGlParameterValue, TCount>;

template <dy::EDyGlParameterName TName> struct TTexParamList;
template <> struct TTexParamList<dy::EDyGlParameterName::TextureMinFilter> final
{
  using _ = dy::EDyGlParameterValue;
  static constexpr TTargetList<6> mList = 
  {
    _::Linear, _::Nearest, 
    _::LinearMipmapLinear, _::LinearMipmapNearest, 
    _::LinearMipmapNearest, _::NearestMipmapNearest
  };
};
template <> struct TTexParamList<dy::EDyGlParameterName::TextureMagFilter> final
{
  using _ = dy::EDyGlParameterValue;
  static constexpr TTargetList<2> mList = { _::Linear, _::Nearest };
};
template <> struct TTexParamList<dy::EDyGlParameterName::TextureWrappingS> final
{
  using _ = dy::EDyGlParameterValue;
  static constexpr TTargetList<4> mList = 
  {
    _::ClampToBorder, _::ClampToEdge, _::Repeat, _::MirroredRepeat
  };
};
template <> struct TTexParamList<dy::EDyGlParameterName::TextureCompareMode> final
{
  using _ = dy::EDyGlParameterValue;
  static constexpr TTargetList<2> mList = { _::CompareRefToTexture, _::CompareNone, };
};
template <> struct TTexParamList<dy::EDyGlParameterName::TextureCompareFunc> final
{
  using _ = dy::EDyGlParameterValue;
  static constexpr TTargetList<8> mList = 
  {
    _::Greater,     _::GreaterEqual, _::Equal,       _::NotEqual,    
    _::LessEqual,   _::Less,        _::Always,      _::Never,       
  };
};

/// @brief
/// @param value
/// @param targetList
/// @return
template <TI32 TValue>
MDY_NODISCARD bool DyIsHaveValueIn(
    _MIN_ const dy::EDyGlParameterValue value,
    _MIN_ const std::array<dy::EDyGlParameterValue, TValue>& targetList)
{
  for (const auto& target : targetList)
  {
    if (value == target) { return true; }
  }
  return false;
}

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy
{

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const EDyGlParameterName& p)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void from_json(_MIN_ const nlohmann::json& j, _MOUT_ EDyGlParameterName& p)
{
  p = DyConvertStringToEDyGlParameterName(j.get<std::string>());
}

EDyGlParameterName DyConvertStringToEDyGlParameterName(_MIN_ const std::string& iString) noexcept
{
  if (iString == "TextureMinFilter")      { return EDyGlParameterName::TextureMinFilter; }
  else if (iString == "TextureMagFilter") { return EDyGlParameterName::TextureMagFilter; }
  else if (iString == "TextureWrappingS") { return EDyGlParameterName::TextureWrappingS; }
  else if (iString == "TextureWrappingT") { return EDyGlParameterName::TextureWrappingT; }
  else if (iString == "TextureCompareMode") { return EDyGlParameterName::TextureCompareMode; }
  else if (iString == "TextureCompareFunc") { return EDyGlParameterName::TextureCompareFunc; }
  else { MDY_UNEXPECTED_BRANCH_BUT_RETURN(EDyGlParameterName::NoneError); }
}

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const EDyGlParameterValue& p)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ EDyGlParameterValue& p)
{
  p = DyConvertStringToEDyGlParameterValue(j.get<std::string>());
}

EDyGlParameterValue DyConvertStringToEDyGlParameterValue(_MIN_ const std::string& iString) noexcept
{
  if (iString == "Nearest")               { return EDyGlParameterValue::Nearest; }
  if (iString == "Linear")                { return EDyGlParameterValue::Linear; }

  if (iString == "NearestMipmapNearest")  { return EDyGlParameterValue::NearestMipmapNearest; }
  if (iString == "LinearMipmapNearest")   { return EDyGlParameterValue::LinearMipmapNearest; }
  if (iString == "NearestMipmapLinear")   { return EDyGlParameterValue::NearestMipmapLinear; }
  if (iString == "LinearMipmapLinear")    { return EDyGlParameterValue::LinearMipmapLinear; }

  if (iString == "ClampToEdge")           { return EDyGlParameterValue::ClampToEdge; }
  if (iString == "ClampToBorder")         { return EDyGlParameterValue::ClampToBorder; }
  if (iString == "Repeat")                { return EDyGlParameterValue::Repeat; }
  if (iString == "MirroredRepeat")        { return EDyGlParameterValue::MirroredRepeat; }

  if (iString == "CompareRefToTexture")   { return EDyGlParameterValue::CompareRefToTexture; }
  if (iString == "CompareNone")           { return EDyGlParameterValue::CompareNone; }

  if (iString == "Greater")               { return EDyGlParameterValue::Greater; }
  if (iString == "GreaterEqual")          { return EDyGlParameterValue::GreaterEqual; }
  if (iString == "Equal")                 { return EDyGlParameterValue::Equal; }
  if (iString == "NotEqual")              { return EDyGlParameterValue::NotEqual; }
  if (iString == "LessEqual")             { return EDyGlParameterValue::LessEqual; }
  if (iString == "Less")                  { return EDyGlParameterValue::Less; }
  if (iString == "Always")                { return EDyGlParameterValue::Always; }
  if (iString == "Never")                 { return EDyGlParameterValue::Never; }
  else { MDY_UNEXPECTED_BRANCH_BUT_RETURN(EDyGlParameterValue::NoneError); }
}

void to_json(_MIN_ nlohmann::json& j, _MINOUT_ const PDyGlTexParameterInformation& p)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyGlTexParameterInformation& p)
{
  for (auto& value : j.items())
  { // Must be one item.
    p.mParameterOption = DyConvertStringToEDyGlParameterName(value.key());
    p.mParameterValue  = DyConvertStringToEDyGlParameterValue(value.value());
  }

  MDY_CALL_ASSERT_SUCCESS(DyCheckTextureParameter(p));
}

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const TTextureParameterList& p)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ TTextureParameterList& p)
{
  for (const auto& paramContainer : j)
  {
    p.emplace_back(paramContainer.get<PDyGlTexParameterInformation>());
  };
}

EDySuccess DyCheckTextureParameter(_MIN_ const PDyGlTexParameterInformation& parameter)
{
  using TName = EDyGlParameterName;
  switch (parameter.mParameterOption)
  {
  case TName::TextureMinFilter:
  {
    return DyIsHaveValueIn(
        parameter.mParameterValue, 
        TTexParamList<TName::TextureMinFilter>::mList) == true ? DY_SUCCESS : DY_FAILURE;
  }
  case TName::TextureMagFilter:
  {
    return DyIsHaveValueIn(
        parameter.mParameterValue, 
        TTexParamList<TName::TextureMagFilter>::mList) == true ? DY_SUCCESS : DY_FAILURE;
  }
  case TName::TextureWrappingS:
  case TName::TextureWrappingT:
  {
    return DyIsHaveValueIn(
        parameter.mParameterValue, 
        TTexParamList<TName::TextureWrappingS>::mList) == true ? DY_SUCCESS : DY_FAILURE;
  }
  case TName::TextureCompareMode:
  {
    return DyIsHaveValueIn(
        parameter.mParameterValue, 
        TTexParamList<TName::TextureCompareMode>::mList) == true ? DY_SUCCESS : DY_FAILURE;
  }
  case TName::TextureCompareFunc:
  {
    return DyIsHaveValueIn(
        parameter.mParameterValue, 
        TTexParamList<TName::TextureCompareFunc>::mList) == true ? DY_SUCCESS : DY_FAILURE;
  }
  default: MDY_UNEXPECTED_BRANCH(); break;
  }
  return DY_FAILURE;
}

EDySuccess DyCheckTextureParameterList(_MIN_ const std::vector<PDyGlTexParameterInformation>& parameterList)
{
  for (const auto& parameterItem : parameterList)
  {
    if (DyCheckTextureParameter(parameterItem) == DY_FAILURE) { return DY_FAILURE; }
  }
  return DY_SUCCESS;
}

} /// ::dy namespace