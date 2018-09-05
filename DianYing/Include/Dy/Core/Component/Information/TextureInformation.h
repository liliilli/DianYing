#ifndef GUARD_DY_CORE_COMPONENT_INFORMATION_TEXTURE_INFORMATION_H
#define GUARD_DY_CORE_COMPONENT_INFORMATION_TEXTURE_INFORMATION_H
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

//!
//! Forward declaration
//!

namespace dy
{
class CDyTextureResource;
}

//!
//! Implementation
//!

namespace dy
{

///
/// @class CDyTextureInformation
/// @brief Texture information class that not serve heap instance
/// but information to create heap instance.
///
class CDyTextureInformation final
{
public:
  CDyTextureInformation(const PDyTextureConstructionDescriptor& textureConstructionDescriptor);

  CDyTextureInformation(const CDyTextureInformation&)            = delete;
  CDyTextureInformation& operator=(const CDyTextureInformation&) = delete;
  CDyTextureInformation(CDyTextureInformation&&)            = default;
  CDyTextureInformation& operator=(CDyTextureInformation&&) = default;
  ~CDyTextureInformation();

  ///
  /// @brief return immutable descriptor information reference.
  ///
  const PDyTextureConstructionDescriptor& GetInformation() const noexcept
  {
    return this->mTextureInformation;
  }

private:
  PDyTextureConstructionDescriptor mTextureInformation;

  void __pfSetNextLevel(CDyTextureResource* ptr) const noexcept;
  mutable CDyTextureResource* mNextLevelPtr = nullptr;

  friend class CDyTextureResource;
  friend class MDyResource;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_COMPONENT_INFORMATION_TEXTURE_INFORMATION_H