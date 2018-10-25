#ifndef GUARD_DY_ELEMENT_DESCRIPTOR_GLOBALENUMS_H
#define GUARD_DY_ELEMENT_DESCRIPTOR_GLOBALENUMS_H
///
/// MIT License
/// Copyright (c) 2018 Jongmin Yun
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.
///

namespace dy
{

///
/// @enum   EDyMetaObjectType
/// @brief  Meta object type for using DDyObjectInformation and PDyPrefabMetaInformation
///
enum class EDyMetaObjectType
{
  Actor,
  SceneScriptor,
  Object,
  NoneError
};

///
/// @brief  Get FDyObject derived class type helper enumration.
/// @param  typeString Object type string from json.
/// @return Object type
///
inline MDY_NODISCARD EDyMetaObjectType DyGetMetaObjectTypeFrom(_MIN_ const std::string& typeString) noexcept
{
  static MDY_SET_IMMUTABLE_STRING(sActor,           "Actor");
  static MDY_SET_IMMUTABLE_STRING(sObject,          "Object");
  static MDY_SET_IMMUTABLE_STRING(sSceneScriptor,   "SceneScriptor");

  if (typeString == sActor)         { return EDyMetaObjectType::Actor; }
  if (typeString == sObject)        { return EDyMetaObjectType::Object; }
  if (typeString == sSceneScriptor) { return EDyMetaObjectType::SceneScriptor; }
  else                              { return EDyMetaObjectType::NoneError; }
}

} /// ::dy namespace

#endif /// GUARD_DY_ELEMENT_DESCRIPTOR_GLOBALENUMS_H