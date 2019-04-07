#ifndef GUARD_DY_CORE_RESOURCE_RESOURCE_FDyTextureGeneralResource_H
#define GUARD_DY_CORE_RESOURCE_RESOURCE_FDyTextureGeneralResource_H
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

#include <Dy/Core/Resource/Resource/AResourceTextureBase.h>

//!
//! Forward declaration
//!

namespace dy
{
class FInformationTextureGeneral;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

/// @class FResourceTextureGeneral
/// @brief Texture resource class that serve heap instance.
class FResourceTextureGeneral final : public AResourceTextureBase
{
public:
  FResourceTextureGeneral(const FInformationTextureGeneral& information);
  virtual ~FResourceTextureGeneral();

private:
  DIVec2 mTextureSize = {};
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_RESOURCE_FDyTextureGeneralResource_H