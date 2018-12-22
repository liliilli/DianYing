#include <precompiled.h>
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

/// Header file
#include <Dy/Builtin/Attachment/UIIntegration/FDyBtAttUiBasic.h>

namespace dy
{

void FDyBtAttUiBasic::ConstructBuffer(_MIN_ PDyGlAttachmentInstanceMetaInfo& property) noexcept
{
  property.mSpecifierName  = sName;
  property.mAttachmentSize = DDyVectorInt2{1280, 720};
  property.mParameterList  =
  {
    PDyGlTexParameterInformation\
    {EDyGlParameterName::TextureMinFilter, EDyGlParameterValue::Nearest},
    {EDyGlParameterName::TextureMagFilter, EDyGlParameterValue::Nearest},
    {EDyGlParameterName::TextureWrappingS, EDyGlParameterValue::ClampToBorder},
    {EDyGlParameterName::TextureWrappingT, EDyGlParameterValue::ClampToBorder},
  };
  property.mBufferFormat  = EDyRenderBufferInternalFormat::RGBA8;
  property.mBorderColor   = DDyColorRGBA{ 0, 0, 0, 0 };
}

} /// ::dy namespace