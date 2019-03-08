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
#include <Dy/Builtin/Attachment/Emissive/FTempBtAtEmissiveResult.h>

namespace dy
{

void FDyBtAttachmentEmissiveResult::ConstructBuffer(PDyGlAttachmentInstanceMetaInfo& oProperty) noexcept
{
  oProperty.mSpecifierName  = sName;
  oProperty.mAttachmentSize = DDyVectorInt2{1280, 720};
  oProperty.mParameterList  =
  {
    PDyGlTexParameterInformation\
    {EDyGlParameterName::TextureMinFilter, EDyGlParameterValue::Nearest},
    {EDyGlParameterName::TextureMagFilter, EDyGlParameterValue::Nearest},
    {EDyGlParameterName::TextureWrappingS, EDyGlParameterValue::Repeat},
    {EDyGlParameterName::TextureWrappingT, EDyGlParameterValue::Repeat},
  };
  oProperty.mBufferFormat = EDyGlBufferDataInternalFormat::RGBA8;
}

} /// ::dy namespace