#ifndef GUARD_DY_CORE_COMPONENT_INTERNAL_TEXTURE_TYPE_H
#define GUARD_DY_CORE_COMPONENT_INTERNAL_TEXTURE_TYPE_H
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

#include <Dy/Helper/Type/Color.h>
#include <Dy/Core/Component/Internal/TextureEnums.h>

namespace dy
{

///
/// @class PDyTextureParameterOption
/// @brief
///
struct PDyTextureParameterOption final
{
  PDyTextureParameterOption(uint32_t option, uint32_t value) : mOption{option}, mValue{value} {};

  const auto& GetOption() const noexcept
  {
    return mOption;
  }

  const auto& GetValue() const noexcept
  {
    return mValue;
  }
private:
  uint32_t mOption = 0;
  uint32_t mValue  = 0;
};

///
/// @class PDyTextureConstructionDescriptor
/// @brief
///
struct PDyTextureConstructionDescriptor final
{
  std::string mTextureName                                        = "";
  std::string mTextureFileLocalPath                               = "";
  std::string mTextureFileAbsolutePath                            = "";
  DColor      mConstantBorderColor                                = DColor::Black;

  std::vector<PDyTextureParameterOption> mTextureParameterOptions;

  bool        mIsEnabledAbsolutePath                              = true;
  bool        mIsEnabledCustomedTextureParameter                  = false;
  bool        mIsEnabledCreateMipmap                              = false;
  EDyTextureStyleType mTextureType                                = EDyTextureStyleType::None;
  EDyTextureMapType   mTextureMapType                             = EDyTextureMapType::Custom;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_COMPONENT_INTERNAL_TEXTURE_TYPE_H