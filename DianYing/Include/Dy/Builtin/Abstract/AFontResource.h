#pragma once
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
#include <Dy/Core/Reflection/RBuiltinResources.h>
#include <Dy/Meta/Information/MetaInfoFont.h>

namespace dy
{

#define MDY_REGISTER_RESOURCE_FONT(__MAType__, __MAIdentifier__) \
  MDY_REGISTER_RESOURCE_WITH_SPECIFIER(__MAType__, __MAIdentifier__); \
  private: \
    void ConstructBuffer(PDyMetaFontInformation::DBuiltin& oInfo) noexcept override final; \
  public: 
  
struct AFontResource : public IResource
{
  virtual ~AFontResource() = default;
  static constexpr auto value = EResourceType::Font;

  virtual void ConstructBuffer(PDyMetaFontInformation::DBuiltin& oInfo) noexcept = 0;

  /// @brief Return meta info, that has buffer pointer of shader fragment.
  std::any GetMetaInfo() override final;
};

} /// ::dy namespace
