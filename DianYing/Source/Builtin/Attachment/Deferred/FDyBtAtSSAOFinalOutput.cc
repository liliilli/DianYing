#include <precompiled.h>
///
/// MIT License
/// Copyright (c) 2018-2019 Jongmin Yun
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.
///

/// Header file
#include <Dy/Builtin/Attachment/Deferred/FDyBtAtSSAOFinalOutput.h>

namespace dy::builtin
{

void FDyBtAtSSAOOutputFinal::ConstructBuffer(_MOUT_ PDyGlAttachmentInstanceMetaInfo& property) noexcept
{
  property.mSpecifierName  = sName;
  property.mAttachmentSize = DDyVectorInt2{1280, 720};
  property.mParameterList  =
  {
    PDyGlTexParameterInformation\
    {EDyGlParameterName::TextureMinFilter, EDyGlParameterValue::Nearest},
    {EDyGlParameterName::TextureMagFilter, EDyGlParameterValue::Nearest},
    {EDyGlParameterName::TextureWrappingS, EDyGlParameterValue::ClampToEdge},
    {EDyGlParameterName::TextureWrappingT, EDyGlParameterValue::ClampToEdge}
  };
  property.mAttachmentType = EDyTextureStyleType::D2;
  property.mBufferFormat   = EDyGlBufferDataInternalFormat::RED8;
}

} /// ::dy::builtin namespace