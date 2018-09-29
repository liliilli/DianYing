#ifndef GUARD_DY_ELELMENT_DESCRIPTOR_LEVELDESCRIPTOR_H
#define GUARD_DY_ELELMENT_DESCRIPTOR_LEVELDESCRIPTOR_H
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

#include <any>
#include <Dy/Helper/Type/Color.h>
#include <nlohmann/json_fwd.hpp>

namespace dy
{

///
/// @enum EDyFDyObjectType
/// @brief Object type
///
enum class EDyFDyObjectType
{
  FDyPawn,
  FDyPostprocessBlock,
  FDyDirectionalLight,
  FDyPointLight,
  FDySpotLight,
  FDyObject,
  FDySceneScriptableObject,
  FDyCamera,
  FDySound,
  FDySoundListener,
  Error
};

///
/// @enum EDyCDyComponentType
/// @brief Component type to attach FDyObject as component
///
enum class EDyCDyComponentType
{
  Script,
  DirectionalLight,
  Etc,
  NoneError
};

///
/// @struct IDyDependencyInformation
/// @brief Dependency information
///
struct IDyDependencyInformation
{
  /// This mType must be EDyCDyComponentType::Script.
  EDyCDyComponentType mType       = EDyCDyComponentType::NoneError;

  /// hashTo must be same to hash string value of DDyObjectInformation instance to be binded.
  std::string         mBindHashTo = MDY_NOT_INITILAIZED_STR;
};

///
/// @struct DDyFdyPawnDependencyInformation
/// @brief Dependency information to DDyObjectInformation::mDependencyInfo when mType is FDyPawn.
///
struct DDyFdyPawnDependencyInformation final : public IDyDependencyInformation
{
  /// Lua script path. Must be valid and can be loaded anytime in runtime.
  std::string         mScriptPath = MDY_NOT_INITILAIZED_STR;
};

///
/// @struct DDyFDyDirLightDependencyInformation
/// @brief Dependency information to DDyObjectInformation::mDependencyInfo when mType is FDyDirectionalLight.
///
struct DDyFDyDirLightDependencyInformation final : public IDyDependencyInformation
{
  /// Normalized light direction vector.
  DDyVector3          mDirection = {};

  /// Tinting diffuse color
  DDyColor            mTintColor = {};

  /// Intensity of light must be set up to 0~.
  TF32                mIntensity = MDY_NOT_INITIALIZED_M1;
};

///
/// @struct DDyObjectInformation
/// @brief object meta information container
///
struct DDyObjectInformation final
{
  ///
  /// @struct DDyObjectInformation::DDyTransform
  /// @brief World transform information.
  ///
  struct DDyTransform final
  {
    /// World position
    DDyVector3        mPosition = {};

    /// Rotation position
    DDyVector3        mRotation = {};

    /// Scale position
    DDyVector3        mScale = {};
  };

  /// Meta index for classification of objects. Might not be used in game runtime.
  TI32              mMetaIndex        = MDY_NOT_INITIALIZED_M1;

  /// Meta object's name.
  std::string       mName             = MDY_NOT_INITILAIZED_STR;

  /// Meta object's hash value to verify it from other information instance.
  std::string       mHashValue        = MDY_NOT_INITILAIZED_STR;

  /// The type (light, pawn, pp block etc...) of object
  EDyFDyObjectType  mType             = EDyFDyObjectType::Error;

  /// Parent name of this object.
  /// @TODO mParentName must be changed to other way to avoid duplicated object metaname. (Index + Name) CRC32 hashing value might be good.
  /// @TODO OR JUST USING METAINDEX...?
  TI32              mParentMetaIndex  = MDY_NOT_INITIALIZED_M1;

  /// Common transform information.
  DDyTransform      mTransform        = {};

  /// Dependency information which are vary along with mType.
  /// If mType is "EDyFDyObjectType::FdyPawn", must be converted to "DDyFdyPawnDependencyInformation".
  /// If mType is ... not implemented yet!
  std::any          mDependencyInfo;
};

///
/// @struct PDyLevelConstructDescriptor
/// @brief Level construction descriptor.
///
struct PDyLevelConstructDescriptor final
{

  // Initial level name
  std::string                       mLevelName                  = MDY_NOT_INITILAIZED_STR;
  // Initial background of scene to create
  DDyColor                          mLevelBackgroundColor       = DDyColor::White;
  // Object information
  std::vector<DDyObjectInformation> mLevelObjectInformations    = {};

  /// Make descriptor
  static PDyLevelConstructDescriptor GetDescriptor(const nlohmann::json& jsonAtlas);
};

} /// ::dy namespace

#endif /// GUARD_DY_ELELMENT_DESCRIPTOR_LEVELDESCRIPTOR_H