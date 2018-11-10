#ifndef GUARD_DY_COMPONENT_CTOR_PDYFONTRENDERER_H
#define GUARD_DY_COMPONENT_CTOR_PDYFONTRENDERER_H
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

//!
//! Forward declaration
//!

namespace dy
{
class FDyText;
class FDyText_Deprecated;
} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy
{

///
/// @struct PDyFontRendererCtorInformation_Deprecated
/// @brief
///
struct PDyFontRendererCtorInformation_Deprecated final
{ /// Font renderer pointer address.
  FDyText_Deprecated* mFontComponentPtr = nullptr;
};

///
/// @struct PDyFontRendererCtorInformation
/// @brief
///
struct PDyFontRendererCtorInformation final
{
  FDyText* mFontComponentPtr = nullptr;
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_CTOR_PDYFONTRENDERER_H