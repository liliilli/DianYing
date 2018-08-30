#ifndef GUARD_DY_CORE_COMPONENT_TEXTURE_H
#define GUARD_DY_CORE_COMPONENT_TEXTURE_H
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

#include <Dy/Core/Component/Internal/TextureType.h>

namespace dy
{

class CDyTextureComponent final
{
public:
  CDyTextureComponent() = default;
  CDyTextureComponent(const CDyTextureComponent&) = delete;
  CDyTextureComponent(CDyTextureComponent&&)      = default;
  CDyTextureComponent& operator=(const CDyTextureComponent&)  = delete;
  CDyTextureComponent& operator=(CDyTextureComponent&&)       = default;
  ~CDyTextureComponent();

  [[nodiscard]]
  EDySuccess pInitializeTextureResource(const PDyTextureConstructionDescriptor& textureConstructionDescriptor);

  ///
  /// @brief
  ///
  FORCEINLINE uint32_t GetTextureId() const noexcept
  {
    return this->mTextureResourceId;
  };

  ///
  /// @brief
  ///
  FORCEINLINE int32_t GetTextureType() const noexcept
  {
    return this->mTextureType;
  }

  ///
  /// @brief
  ///
  FORCEINLINE int32_t GetTextureWidth() const noexcept
  {
    return this->mTextureWidth;
  }

  ///
  /// @brief
  ///
  FORCEINLINE int32_t GetTextureHeight() const noexcept
  {
    return this->mTextureHeight;
  }

private:
  /// Valid texture id must not be 0.
  // @todo JUST ONLY OPENGL
  int32_t   mTextureType        = GL_TEXTURE_2D;
	GLuint    mTextureResourceId  = 0;
  int32_t   mTextureWidth       = MDY_NOT_INITIALIZED_M1;
  int32_t   mTextureHeight      = MDY_NOT_INITIALIZED_M1;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_COMPONENT_TEXTURE_H