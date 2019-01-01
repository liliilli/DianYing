#ifndef GUARD_DY_ELEMENT_TYPE_PDYACTORCREATIONDESCIRPTOR_H
#define GUARD_DY_ELEMENT_TYPE_PDYACTORCREATIONDESCIRPTOR_H
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

#include <Dy/Element/Type/DDyTransform.h>
#include <Dy/Element/Abstract/Actor/ADyActorBinderContainer.h>

namespace dy
{

struct PDyActorCreationDescriptor final : public ADyActorBinderContainer<PDyActorCreationDescriptor>
{
  /// @brief auto generated actor specifier name.
  std::string   mActorSpecifierName       = MDY_INITIALIZE_EMPTYSTR;
  /// @brief valid prefab specifier name.
  std::string   mPrefabSpecifierName      = MDY_INITIALIZE_EMPTYSTR;
  /// @brief Full length of parent name from `root`.
  std::string   mParentFullSpecifierName  = MDY_INITIALIZE_EMPTYSTR;
  /// @brief Vaild or empty tag name.
  std::string   mObjectTag                = MDY_INITIALIZE_EMPTYSTR;
  /// @brief Prefab tag will be overriden by mObjectTag if true.
  bool          mIsOverridePrefabTag      = false;
  DDyTransform  mTransform {};
  bool          mIsDoSweep = false;
};

} /// ::dy namespace

#endif /// GUARD_DY_ELEMENT_TYPE_PDYACTORCREATIONDESCIRPTOR_H