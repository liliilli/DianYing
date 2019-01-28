#ifndef GUARD_DY_META_TYPE_EDYRESOURCETYPE_H
#define GUARD_DY_META_TYPE_EDYRESOURCETYPE_H
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

namespace dy
{

enum class EDyResourceType
{
  Script,
  Mesh,
  Model,
  GLShader,
  GLAttachment,
  GLFrameBuffer,
  Texture,
  Material,
  WidgetMeta,
  Sound,
  Skeleton,       // Skeleton is dynamic model skeleton data type for begin used in model animation and model (not for each mesh)
  AnimationScrap, // AnimationScrap is data of model animation, but not reformed and just have binary data for animation.

  __MeshVBO,
  NoneError
};

} /// ::dy namespace

#endif /// GUARD_DY_META_TYPE_EDYRESOURCETYPE_H