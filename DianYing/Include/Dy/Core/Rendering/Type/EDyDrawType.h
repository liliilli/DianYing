#ifndef GUARD_DY_CORE_RENDERING_TYPE_EDYDRAWTYPE_H
#define GUARD_DY_CORE_RENDERING_TYPE_EDYDRAWTYPE_H
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

namespace dy
{

/// @enum EDyDrawType
/// @brief determine draw type.
enum class EDyDrawType 
{
  Point,
  Line,
  LineStrip,
  LineLoop,
  Triangle,
  TriangleStip,
  TriangleFan,
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RENDERING_TYPE_EDYDRAWTYPE_H