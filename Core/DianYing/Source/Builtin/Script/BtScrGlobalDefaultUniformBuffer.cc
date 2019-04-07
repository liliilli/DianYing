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

#include <Dy/Builtin/Script/BtScrGlobalDefaultUniformBuffer.h>
#include <Dy/Management/MSetting.h>
#include <Dy/Core/Reflection/RGlobalInstanceManager.h>
#include <Dy/Component/Internal/Lights/DUboDirLight.h>
#include <Dy/Component/Internal/Lights/DUboPointLight.h>
#include <Dy/Management/MFont.h>
#include <Dy/Management/Rendering/MUniformBufferObject.h>
#include <Dy/Component/Internal/Camera/DUboCameraBlock.h>

namespace dy
{

void BtScrGlobalDefaultUniformBuffer::OnStart()
{
  // Create ubo information for "CameraBlock"
  auto& uboManager = MUniformBufferObject::GetInstance();
  {
    PDyUboConstructionDescriptor desc = {};
    desc.mBindingIndex = 0;
    desc.mUboSpecifierName = sUboCameraBlock;
    desc.mBufferDrawType = EDyBufferDrawType::DynamicDraw;
    desc.mUboElementSize = sizeof(DUboCameraBlock);
    desc.mUboArraySize = 1;
    MDY_CALL_ASSERT_SUCCESS(uboManager.CreateUboContainer(desc));
  }
  {
    PDyUboConstructionDescriptor desc = {};
    desc.mBindingIndex = 1;
    desc.mUboSpecifierName = "dyBtUboDirLight";
    desc.mBufferDrawType = EDyBufferDrawType::DynamicDraw;
    desc.mUboElementSize = sizeof(DUboDirectionalLight);
    desc.mUboArraySize = 1;
    MDY_CALL_ASSERT_SUCCESS(uboManager.CreateUboContainer(desc))
  }
  {
    PDyUboConstructionDescriptor desc = {};
    desc.mBindingIndex = 2;
    desc.mUboSpecifierName = "dyBtUboDirShadow";
    desc.mBufferDrawType = EDyBufferDrawType::DynamicDraw;
    desc.mUboElementSize = sizeof(DDyUboDirShadow);
    desc.mUboArraySize = 1;
    MDY_CALL_ASSERT_SUCCESS(uboManager.CreateUboContainer(desc))
  }

  MFont::GetInstance().CreateFontResourceContainer("dyBtFtDefaultGothic");
}

void BtScrGlobalDefaultUniformBuffer::OnEnd()
{
  auto& uboManager = MUniformBufferObject::GetInstance();

  //MDY_CALL_ASSERT_SUCCESS(uboManager.RemoveUboContainer("dyBtUboPointLight"));
  MDY_CALL_ASSERT_SUCCESS(uboManager.RemoveUboContainer("dyBtUboDirShadow"));
  MDY_CALL_ASSERT_SUCCESS(uboManager.RemoveUboContainer("dyBtUboDirLight"));
  MDY_CALL_ASSERT_SUCCESS(uboManager.RemoveUboContainer(sUboCameraBlock));
}

} /// ::dy namespace

