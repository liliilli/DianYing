#ifndef GUARD_DY_BUILTIN_ABSTRACT_ADYGLFRAMEBUFFERRRESOURCE_H
#define GUARD_DY_BUILTIN_ABSTRACT_ADYGLFRAMEBUFFERRRESOURCE_H
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

#include <Dy/Builtin/Interface/IResource.h>
#include <Dy/Meta/Type/EResourceType.h>
#include <Dy/Meta/Information/MetaInfoFrameBuffer.h>

namespace dy
{

/// @macro MDY_REGISTER_RESOURCE_FRAMEBUFFER
/// @brief Register OpenGL framebuffer meta information source.
#define MDY_REGISTER_RESOURCE_FRAMEBUFFER(__MAType__, __MASpecifierName__) \
  MDY_REGISTER_RESOURCE_WITH_SPECIFIER(__MAType__, __MASpecifierName__) \
  private: \
  void ConstructBuffer(PDyGlFrameBufferInstanceMetaInfo& property) noexcept; \
  public: \
  __MAType__() \
  { \
    this->ConstructBuffer(this->MIOMeta); \
    this->SetDefaultSettings(); \
  }

struct AGLFrameBufferResource : public IResource
{
protected:
  /// @brief Temporary Framebuffer object meta information to be inserted in Dy system.
  PDyGlFrameBufferInstanceMetaInfo MIOMeta{};
  /// @brief Set default setting to be 
  /// specified condition of framebuffer object description.
  void SetDefaultSettings();
public:
  static constexpr auto value = EResourceType::GLFrameBuffer;

  /// @brief Get meta information that has frame buffer meta information.
  std::any GetMetaInfo() override final;
};

} /// ::dy namespace

#endif /// GUARD_DY_BUILTIN_ABSTRACT_ADYGLFRAMEBUFFERRRESOURCE_H