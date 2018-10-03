#include <precompiled.h>
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

/// Header file
#include <Dy/Element/Actor.h>
#include <Dy/Management/MetaInfoManager.h>
#include <Dy/Component/CDyTransform.h>
#include <Dy/Component/Internal/CDyEmptyTransform.h>

//!
//! Forward declaration
//!

namespace dy
{

///
DDyTransformMetaInformation sDefaultTransformMetaInformation;

} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

EDySuccess FDyActor::Initialize(_MIN_ const DDyObjectInformation& objectMetaDesc)
{
  bool isTransformCreated = false;

  // Create components
  for (const auto& [type, componentInfo] : objectMetaDesc.mMetaComponentInfo)
  {
    switch (type)
    {
    default: PHITOS_UNEXPECTED_BRANCH(); break;
    case EDyComponentMetaType::Transform:
    {
      const auto& desc = std::any_cast<const DDyTransformMetaInformation&>(componentInfo);
      [[maybe_unused]] auto transformComponentPtr = this->AddComponent<CDyTransform>(desc);

      isTransformCreated = true;
    }
    break;
    case EDyComponentMetaType::Script:
    {
      const auto& desc = std::any_cast<const DDyScriptMetaInformation&>(componentInfo);
      [[maybe_unused]] auto scriptComponentPtr = this->AddComponent<CDyScript>(desc);
    }
    break;
    case EDyComponentMetaType::DirectionalLight:
    {
      const auto& desc = std::any_cast<const DDyDirectionalLightMetaInformation&>(componentInfo);
      (void)desc;
      //const auto& directionalLight = std::any_cast<const &>(componentInfo);
    }
    break;
    }
  }

  // Create CDyEmptyTransform when not having CDyTransform.
  if (isTransformCreated == false)
  {
    [[maybe_unused]] auto ptr = this->AddComponent<CDyEmptyTransform>(sDefaultTransformMetaInformation);
  }

  return DY_SUCCESS;
}

EDySuccess FDyActor::Release()
{


  return DY_SUCCESS;
}

void FDyActor::SetParent(NotNull<FDyActor*> validParentRawPtr) noexcept
{
  mParentFDyActorRawPtr = validParentRawPtr;
  MDY_LOG_WARNING("NOT IMPLEMENTED {}", "FDyActor::SetParent");
}

void FDyActor::SetParentRelocateTransform(NotNull<FDyActor*> validParentRawPtr) noexcept
{
  this->SetParent(validParentRawPtr);
  MDY_LOG_WARNING("NOT IMPLEMENTED {}", "FDyActor::SetParentRelocateTransform");
}

void FDyActor::SetParentToRoot() noexcept
{

  MDY_LOG_WARNING("NOT IMPLEMENTED {}", "FDyActor::SetParentToRoot");
}

void FDyActor::SetParentToRootRelocateTransform() noexcept
{
  this->SetParentToRoot();
  MDY_LOG_WARNING("NOT IMPLEMENTED {}", "FDyActor::SetParentToRootRelocateTransform");
}

std::optional<CDyScript*> FDyActor::GetScriptComponent(const std::string& scriptName) noexcept
{
  PHITOS_ASSERT(!scriptName.empty(), "scriptName must not be empty at FDyActor::GetScriptComponent()");
  PHITOS_NOT_IMPLEMENTED_ASSERT();
  return std::nullopt;
}

} /// ::dy namespace