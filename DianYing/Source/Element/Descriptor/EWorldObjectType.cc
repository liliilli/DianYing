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

#include <Dy/Element/Descriptor/EWorldObjectType.h>
#include <nlohmann/json.hpp>

//!
//! Forward declaration
//!

namespace
{

///
/// @brief  Get AWorldObject derived class type helper enumration.
/// @param  typeString Object type string from json.
/// @return Object type
///
MDY_NODISCARD dy::EWorldObjectType
DyGetMetaObjectTypeFrom(_MIN_ const std::string& typeString) noexcept
{
  using EDyMetaObjectType = dy::EWorldObjectType;

  static MDY_SET_IMMUTABLE_STRING(sActor,           "Actor");
  static MDY_SET_IMMUTABLE_STRING(sObject,          "Object");
  static MDY_SET_IMMUTABLE_STRING(sSceneScriptor,   "SceneScriptor");

  if (typeString == sActor)         { return EDyMetaObjectType::Actor; }
  if (typeString == sObject)        { return EDyMetaObjectType::Object; }
  if (typeString == sSceneScriptor) { return EDyMetaObjectType::SceneScriptor; }
  else                              { return EDyMetaObjectType::NoneError; }
}

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy
{

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const EWorldObjectType& p)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ EWorldObjectType& p)
{
  p = DyGetMetaObjectTypeFrom(j.get<std::string>());
}

} /// ::dy namespace
