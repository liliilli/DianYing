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

/// Header file
#include <Dy/Component/CSkybox.h>
#include <Dy/Core/Resource/Resource/AResourceTextureBase.h>
#include <Dy/Management/MWorld.h>

namespace dy
{

/*
 *{
    "Type": "Skybox", "Activated": true,
    "Details": { "Exposure" : 1.0, "Rotation" : 0.0, "CubemapSpecifier": "Skybox_cloudtop_up" }
  }
 */

EDySuccess CSkybox::Initialize(_MIN_ const PDySkyboxComponentMetaInfo& descriptor)
{
  // Get properties.
  this->mExposure         = descriptor.mDetails.mExposure;
  this->mRotationDegree   = descriptor.mDetails.mRotation; 
  this->mTextureSpecifier = descriptor.mDetails.mCubemapSpecifier;
  this->mTintColor        = descriptor.mDetails.mTintColor;

  // Activation
  if (descriptor.mInitiallyActivated == true) { this->Activate(); }
  return EDySuccess::DY_SUCCESS;
}

void CSkybox::Release()
{
  if (this->IsComponentActivated() == true) { this->Deactivate(); }
}

std::string CSkybox::ToString()
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  return "";
}

TDyResourceBinderTexture& CSkybox::MDY_PRIVATE(GetTextureBinderReference)() noexcept
{
  return this->mBinderTexture;
}

void CSkybox::SetExposure(_MIN_ TF32 iValue) noexcept
{
  this->mExposure = iValue;
}

TF32 CSkybox::GetExposure() const noexcept
{
  return this->mExposure;
}

void CSkybox::SetRotationDegree(_MIN_ TF32 iDegree) noexcept
{
  this->mRotationDegree = iDegree;
}

TF32 CSkybox::GetRotationDegree() const noexcept
{
  return this->mRotationDegree;
}

void CSkybox::SetTintColor(_MIN_ const DColorRGB& iTintColor) noexcept
{
  this->mTintColor = iTintColor;
}

const DColorRGB& CSkybox::GetTintColor() const noexcept
{
  return this->mTintColor;
}

void CSkybox::TryActivateInstance()
{
  // If binder textrue is already binded to something,
  if (this->mBinderTexture.IsResourceExist() == true)
  {
    // But texture name is not same to component's texture name,
    // Try to require another `cubemap` texture.
    if (this->mBinderTexture->GetSpecifierName() != this->mTextureSpecifier)
    {
      this->mBinderTexture.TryRequireResource(this->mTextureSpecifier);
    }
  }
  else
  { // If not bound to anything, try require resource.
    this->mBinderTexture.TryRequireResource(this->mTextureSpecifier);
  }

  // And attach to world's activated skybox component list.
  auto& worldManager = MWorld::GetInstance();
  worldManager.MDY_PRIVATE(BindActiveSkybox)(*this);
}

void CSkybox::TryDeactivateInstance()
{
  // Detach from world's activated skybox component list.
  auto& worldManager = MWorld::GetInstance();
  const auto flag = worldManager.MDY_PRIVATE(UnbindActiveSkybox)(*this);
  // Procedure must be succeeded.
  MDY_ASSERT_MSG_FORCE(flag == EDySuccess::DY_SUCCESS, "Unexpected error occurred.");
}

} /// ::dy namespace