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

#include <Dy/Management/Type/UBOInformation.h>
#include <Dy/Include/GlInclude.h>

namespace dy
{

TGlEnum DyGetBufferDrawTypeValue(EDyBufferDrawType type) noexcept
{
  TGlEnum retValue = GL_NONE;
  switch (type)
  {
  case EDyBufferDrawType::StaticDraw:   retValue = GL_STATIC_DRAW;  break;
  case EDyBufferDrawType::DynamicDraw:  retValue = GL_DYNAMIC_DRAW; break;
  }
  return retValue;
}

} /// ::dy namespace