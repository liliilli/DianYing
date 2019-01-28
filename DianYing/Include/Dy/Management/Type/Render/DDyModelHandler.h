#ifndef GUARD_DY_MANAGEMENT_TYPE_RENDER_DDyModelHandler_H
#define GUARD_DY_MANAGEMENT_TYPE_RENDER_DDyModelHandler_H
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

#include <unordered_map>
#include <Dy/Core/Resource/Type/TDyResourceBinder.h>

//!
//! Forward declaration
//!

namespace dy
{
class FDyActor;
class CDyModelFilter;
class CDyModelRenderer;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

/// @struct DDyModelHandler
/// @brief Model handler.
struct DDyModelHandler final
{
public:
  

private:
  /// @struct DActorInfo
  /// @brief 
  struct DActorInfo final
  {
    FDyActor*         mPtrValidActor      = nullptr;
    CDyModelFilter*   mPtrCompModelFilter = nullptr;
    CDyModelRenderer* mPtrModelRenderer   = nullptr;
  };

  /// Only specifies one valid model.
  TDyLResourceBinderModel                   mModelBinder;
  /// Have pointer of actor and components information.
  std::unordered_map<FDyActor*, DActorInfo> mActorInformation;
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_TYPE_RENDER_DDyModelHandler_H