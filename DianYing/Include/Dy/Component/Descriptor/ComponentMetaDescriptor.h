#ifndef GUARD_DY_COMPONENT_DESCRIPTOR_COMPONENTMETADESCRIPTOR_H
#define GUARD_DY_COMPONENT_DESCRIPTOR_COMPONENTMETADESCRIPTOR_H
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

#include <string>
#include <Dy/Helper/Type/Vector3.h>
#include <Dy/Helper/Type/Color.h>

namespace dy
{

///
/// @enum   EDyComponentMetaType
/// @brief  Meta type for component
///
enum class EDyComponentMetaType
{
  Transform,
  Script,
  DirectionalLight,
  NoneError
};

///
/// @struct IDyMetaInformation
/// @brief Dependency information
///
struct IDyMetaInformation
{
  /// This mType must be EDyCDyComponentType::Script.
  EDyComponentMetaType mType       = EDyComponentMetaType::NoneError;
  /// hashTo must be same to hash string value of DDyObjectInformation instance to be binded.
  std::string          mBindHashTo = MDY_INITILAIZE_EMPTYSTR;
};

///
/// @struct DDyScriptMetaInformation
/// @brief Dependency information to DDyObjectInformation::mMetaComponentInfo when mType is FDyPawn.
///
struct DDyScriptMetaInformation final : public IDyMetaInformation
{
  /// Lua script path. Must be valid and can be loaded anytime in runtime.
  std::string      mScriptPath = MDY_INITILAIZE_EMPTYSTR;
};

///
/// @struct DDyDirectionalLightMetaInformation
/// @brief Dependency information to DDyObjectInformation::mMetaComponentInfo when mType is FDyDirectionalLight.
///
struct DDyDirectionalLightMetaInformation final : public IDyMetaInformation
{
  /// Normalized light direction vector.
  DDyVector3          mDirection = {};
  /// Tinting diffuse color
  DDyColor            mTintColor = {};
  /// Intensity of light must be set up to 0~.
  TF32                mIntensity = MDY_INITIALIZE_DEFINT;
};

///
/// @struct DDyTransformMetaInformation
/// @brief  World transform information.
///
struct DDyTransformMetaInformation final : public IDyMetaInformation
{
  /// Local position
  DDyVector3        mLocalPosition  = {};
  /// Local rotation
  DDyVector3        mLocalRotation  = {};
  /// Local scale
  DDyVector3        mLocalScale     = DDyVector3{1, 1, 1};
  /// World position
  DDyVector3        mWorldPosition  = {};
  /// World rotation
  DDyVector3        mWorldRotation  = {};
  /// World scale
  DDyVector3        mWorldScale     = DDyVector3{1, 1, 1};
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_DESCRIPTOR_COMPONENTMETADESCRIPTOR_H