#include <precompiled.h>
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

/// Header file
#include <Dy/Builtin/Attachment/CSM/FDyBtAtLight.h>

namespace dy::builtin
{

void FDyBtAtCSMLight::ConstructBuffer(_MOUT_ PDyGlAttachmentInstanceMetaInfo& property) noexcept
{
  property.mSpecifierName  = sName;
  property.mAttachmentSize = DDyVectorInt2{2048, 2048};
  property.mParameterList  =
  {
    PDyGlTexParameterInformation\
    {EDyGlParameterName::TextureMinFilter, EDyGlParameterValue::Linear},
    {EDyGlParameterName::TextureMagFilter, EDyGlParameterValue::Linear},
    {EDyGlParameterName::TextureCompareMode, EDyGlParameterValue::CompareRefToTexture},
    {EDyGlParameterName::TextureCompareFunc, EDyGlParameterValue::LessEqual},
  };
  property.mAttachmentType  = EDyTextureStyleType::D2ShadowArray;
  property.mBufferFormat    = EDyGlBufferDataInternalFormat::DEPTH32;
  property.mDepthNumber     = 4;
}

} /// ::dy::builtin namespace