#ifndef GUARD_DY_META_TYPE_EDYRESOURCETYPE_H
#define GUARD_DY_META_TYPE_EDYRESOURCETYPE_H
///
/// MIT License
/// Copyright (c) 2018 Jongmin Yun
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
  Model,
  GLShader,
  Texture,
  Material,
  WidgetMeta,

  __ModelVBO,
  NoneError
};

} /// ::dy namespace

#endif /// GUARD_DY_META_TYPE_EDYRESOURCETYPE_H