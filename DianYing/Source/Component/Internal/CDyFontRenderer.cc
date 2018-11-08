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
#include <Dy/Component/Internal/CDyFontRenderer.h>

namespace dy
{

EDySuccess CDyFontRenderer::Initialize(const DDyFontRendererMetaInformation& descriptor)
{
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(descriptor.mFontComponentPtr), "descriptor.mFontComponentPtr must not be null.");
  this->mFontObjectRawPtr = descriptor.mFontComponentPtr;

  return DY_SUCCESS;
}

void CDyFontRenderer::Release()
{
  this->mFontObjectRawPtr = nullptr;
}

} /// ::dy namespace