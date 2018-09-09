#ifndef GUARD_DY_CORE_COMPONENT_RENDERER_H
#define GUARD_DY_CORE_COMPONENT_RENDERER_H
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
struct  PDyRendererConsturctionDescriptor;
class   CDyModelResource;
class   CDyMaterialResource;
class   CDyMeshResource;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

///
/// @class CDyMeshRenderer
/// @brief Renderer component have reference of shader and model instance with material own properties.
///
class CDyMeshRenderer
{
public:
  CDyMeshRenderer() = default;
  CDyMeshRenderer(const CDyMeshRenderer&) = delete;
  CDyMeshRenderer(CDyMeshRenderer&&)      = default;
  CDyMeshRenderer& operator=(const CDyMeshRenderer&)  = delete;
  CDyMeshRenderer& operator=(CDyMeshRenderer&&)       = default;
  //~CDyMeshRenderer();

  ///
  /// @brief
  ///
  [[nodiscard]]
  EDySuccess pfInitialize(const PDyRendererConsturctionDescriptor& desc);

  ///
  /// @brief Render mesh.
  ///
  void Render();

private:
  CDyModelResource*                 mModelReferencePtr    = nullptr;
  std::vector<CDyMaterialResource*> mMaterialResourcePtr  = {};

  struct DDyBindingInformation final
  {
    CDyMeshResource*      mSubmeshResource  = nullptr;
    CDyMaterialResource*  mMaterialResource = nullptr;
  };

  std::vector<DDyBindingInformation> mMeshMaterialPtrBindingList = {};
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_COMPONENT_RENDERER_H