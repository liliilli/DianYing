#ifndef GUARD_DY_COMPONENT_INTENRAL_TEXTURE_ENUMS_H
#define GUARD_DY_COMPONENT_INTENRAL_TEXTURE_ENUMS_H
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

///
/// @enum EDyTextureMapType
/// @brief Texture mapping type
///
enum class EDyTextureMapType : unsigned char
{
  Custom,
  Diffuse,
  Specular,
  Ambient,
  Height,
  Displacement,
  Emissive,
  LightMap,
  Normal,
  Reflection,
  Shininess
};

///
/// @enum EDyTextureStyleType
/// @brief Texture loading type
///
enum class EDyTextureStyleType : unsigned char
{
  None,
  D1,
  D2,
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_INTENRAL_TEXTURE_ENUMS_H