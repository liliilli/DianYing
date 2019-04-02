#ifndef GUARD_DY_CORE_RESOURCE_INFORMATION_AResourceFrameBufferBase_H
#define GUARD_DY_CORE_RESOURCE_INFORMATION_AResourceFrameBufferBase_H
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

#include <Dy/Core/Resource/Type/TInformationBinder.h>
#include <Dy/Core/Resource/Type/TResourceBinder.h>
#include <Dy/Management/Type/AttachmentInformation.h>

//!
//! Forward declaration
//!

namespace dy
{
class FInformationFrameBuffer;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

/// @class AResourceFrameBufferBase
/// @brief Frame buffer resource type (abstract)
class AResourceFrameBufferBase 
{
public:
  AResourceFrameBufferBase() = default;
  virtual ~AResourceFrameBufferBase() = 0;

  /// @brief Get specifier name of framebuffer resource.
  const std::string& GetSpecifierName();
  /// @brief Get framebuffer id.
  virtual TU32 GetTargetFrameBufferId() const noexcept = 0;
  /// @brief Get framebuffer size.
  const DIVec2& GetFrameBufferSize();

  /// @brief Bind Framebuffer.
  EDySuccess virtual BindFrameBuffer() const noexcept = 0;
  /// @brief Unbind framebuffer and revert to default framebuffer.
  void UnbindFrameBuffer() const noexcept; 

  /// @brief Push framebuffer's internal gl states.
  void PushGlobalStates();
  /// @brief Pop framebuffer's internal gl states.
  void PopGlobalStates();

  /// @brief Swap.
  void virtual Swap() = 0;

protected:
  std::string mSpecifierName;
  DIVec2 mFrameBufferSize  = {};

  using TAttachmentBinder = std::unique_ptr<TDyResourceBinderAttachment>;
  using TAttachmentBinderList = std::vector<TAttachmentBinder>;

  TDyInformationBinderFrameBuffer mBinderFrameBuffer;
  TAttachmentBinderList           mBinderAttachmentList   = {};
  TBlendingEquationList           mAttachmentBlendingList = {};
  TDyResourceBinderAttachment     mBinderDepthBuffer      = {};
};

inline AResourceFrameBufferBase::~AResourceFrameBufferBase() = default;

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_INFORMATION_AResourceFrameBufferBase_H
