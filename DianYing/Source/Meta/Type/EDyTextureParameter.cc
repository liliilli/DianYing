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

///
/// @brief
/// @param value
/// @param targetList
/// @return
///
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

void to_json(nlohmann::json& j, const EDyGlParameterName& p)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void from_json(const nlohmann::json& j, EDyGlParameterName& p)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const EDyGlParameterValue& p)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ EDyGlParameterValue& p)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void to_json(_MIN_ nlohmann::json& j, _MINOUT_ const PDyGlTexParameterInformation& p)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyGlTexParameterInformation& p)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
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
  static constexpr TTargetList<6> sTextureMinFilter = {
    EDyGlParameterValue::Linear, EDyGlParameterValue::Nearest,
    EDyGlParameterValue::LinearMipmapLinear, EDyGlParameterValue::LinearMipmapNearest,
    EDyGlParameterValue::LinearMipmapNearest, EDyGlParameterValue::NearestMipmapNearest
  };

  static constexpr TTargetList<2> sTextureMagFilter = {
    EDyGlParameterValue::Linear, EDyGlParameterValue::Nearest,
  };

  static constexpr TTargetList<4> sTextureWrappingSTR = {
    EDyGlParameterValue::ClampToBorder, EDyGlParameterValue::ClampToEdge,
    EDyGlParameterValue::Repeat, EDyGlParameterValue::MirroredRepeat
  };

  switch (parameter.mParameterOption)
  {
  case EDyGlParameterName::TextureMinFilter:
    return DyIsHaveValueIn(parameter.mParameterValue, sTextureMinFilter) ? DY_SUCCESS : DY_FAILURE;
  case EDyGlParameterName::TextureMagFilter:
    return DyIsHaveValueIn(parameter.mParameterValue, sTextureMagFilter) ? DY_SUCCESS : DY_FAILURE;
  case EDyGlParameterName::TextureWrappingS:
  case EDyGlParameterName::TextureWrappingT:
    return DyIsHaveValueIn(parameter.mParameterValue, sTextureWrappingSTR) ? DY_SUCCESS : DY_FAILURE;
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