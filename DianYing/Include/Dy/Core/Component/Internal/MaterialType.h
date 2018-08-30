#ifndef GUARD_DY_CORE_COMPONENT_INTERNAL_MATERIAL_TYPE_H
#define GUARD_DY_CORE_COMPONENT_INTERNAL_MATERIAL_TYPE_H
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

#include <vector>

//!
//! Forward declaration
//!

namespace dy
{
class CDyShaderResource;
class CDyTextureComponent;
}

//!
//! Implementation
//!

namespace dy
{

///
/// @enum EDyMaterialBlendMode
/// @brief
///
enum class EDyMaterialBlendMode
{
  Opaque,
  Translucent,
};

///
/// @struct
/// @brief
///
struct PDyMaterialConstructionDescriptor final
{
  CDyShaderResource* mShaderComponentPtr = nullptr;
  std::vector<CDyTextureComponent*> mTextureComponents;

  EDyMaterialBlendMode mBlendMode = EDyMaterialBlendMode::Opaque;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_COMPONENT_INTERNAL_MATERIAL_TYPE_H