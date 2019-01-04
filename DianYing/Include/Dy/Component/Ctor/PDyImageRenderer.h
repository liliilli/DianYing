#ifndef GUARD_DY_COMPONENT_CTOR_PDYIMAGERENDERER_H
#define GUARD_DY_COMPONENT_CTOR_PDYIMAGERENDERER_H
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
class FDyImage;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

///
/// @struct PDyImageRendererCtorInformation
/// @brief Descriptor for `FDyImage` widget object.
///
struct PDyImageRendererCtorInformation final
{
  FDyImage* mPtrUiObject = nullptr;
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_CTOR_PDYIMAGERENDERER_H