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
/// @struct PDyLevelConstructDescriptor
/// @brief Level construction descriptor.
///
struct PDyLevelConstructDescriptor final
{
  ///
  /// @struct DDyObjectInformation
  /// @brief object meta information container
  ///
  struct DDyObjectInformation final
  {
    /// Meta index for classification of objects. Might not be used in game runtime.
    TI32        mMetaIndex  = MDY_NOT_INITIALIZED_M1;
    /// Meta object's name.
    std::string mName       = MDY_NOT_INITILAIZED_STR;
    /// The type (light, pawn, pp block etc...) of object
    std::string mType       = MDY_NOT_INITILAIZED_STR;
    /// Parent name of this object.
    /// @TODO mParentName must be changed to other way to avoid duplicated object metaname. (Index + Name) CRC32 hashing value might be good.
    /// @TODO OR JUST USING METAINDEX...?
    TI32        mParentMetaIndex = MDY_NOT_INITIALIZED_M1;

    DDyVector3  mPosition   = {};
    DDyVector3  mRotation   = {};
    DDyVector3  mScale      = {};
  };

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