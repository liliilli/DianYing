#ifndef GUARD_DY_CORE_RENDERING_BASICRENDERER_H
#define GUARD_DY_CORE_RENDERING_BASICRENDERER_H
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

#include <vector>
#include <Dy/Core/Resource/Type/TDyResourceBinder.h>
#include "Dy/Management/Type/Render/DDyModelHandler.h"

//!
//! Forward declaration
//!

namespace dy
{
class   CDyCamera;
class   CDyModelRenderer;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

///
/// @class FDyBasicRenderer
/// @brief
///
class FDyBasicRenderer final
{
public:
  MDY_NOT_COPYABLE_MOVEABLE_PROPERTIES(FDyBasicRenderer);
  FDyBasicRenderer();
  ~FDyBasicRenderer();

  /// @brief Update Properties of renderer object.
  void PreRender();

  /// @brief Rendering deferred contexts to default framebuffer.
  void RenderScreen(
      _MIN_ DDyModelHandler::DActorInfo& iRefRenderer,
      _MIN_ FDyMeshResource& iRefMesh, 
      _MIN_ FDyMaterialResource& iRefMaterial);

  /// @brief Clear properties of given framebuffer.
  void Clear();

private:

  TDyResourceBinderFrameBuffer mBinderFrameBuffer{"dyBtBasicRender"};

  friend class FDyDeferredRenderingMesh;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RENDERING_BASICRENDERER_H