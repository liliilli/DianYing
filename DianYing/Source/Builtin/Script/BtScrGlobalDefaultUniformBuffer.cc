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
#include <Dy/Management/LoggingManager.h>
#include <Dy/Management/SettingManager.h>
#include <Dy/Core/Reflection/RDyGlobalInstanceManager.h>
#include <Dy/Builtin/GlobalInstance/FDyBtGiDebugStatus.h>
#include <Dy/Management/FontManager.h>
#include <Dy/Management/Rendering/UniformBufferObjectManager.h>
#include <Dy/Component/CDyCamera.h>
#include <Dy/Component/CDyDirectionalLight.h>

namespace dy
{

void BtScrGlobalDefaultUniformBuffer::OnStart()
{
  // Create ubo information for "CameraBlock"
  {
    auto& uboManager = MDyUniformBufferObject::GetInstance();
    PDyUboConstructionDescriptor desc = {};
    desc.mBindingIndex = 0;
    desc.mUboSpecifierName = sUboCameraBlock;
    desc.mBufferDrawType = EDyBufferDrawType::DynamicDraw;
    desc.mUboElementSize = sizeof(DDyUboCameraBlock);
    desc.mUboArraySize = 1;
    MDY_CALL_ASSERT_SUCCESS(uboManager.CreateUboContainer(desc));
  }

  {
    auto& uboManager = MDyUniformBufferObject::GetInstance();
    PDyUboConstructionDescriptor desc = {};
    desc.mBindingIndex = 1;
    desc.mUboSpecifierName = "dyBtUboDirLight";
    desc.mBufferDrawType = EDyBufferDrawType::DynamicDraw;
    desc.mUboElementSize = sizeof(DDyUboDirectionalLight);
    desc.mUboArraySize = 1;
    MDY_CALL_ASSERT_SUCCESS(uboManager.CreateUboContainer(desc))
  }
 
  MDyFont::GetInstance().CreateFontResourceContainer("Arial");
}

void BtScrGlobalDefaultUniformBuffer::OnEnd()
{
  auto& uboManager = MDyUniformBufferObject::GetInstance();

  MDY_CALL_ASSERT_SUCCESS(uboManager.RemoveUboContainer("dyBtUboDirLight"));
  MDY_CALL_ASSERT_SUCCESS(uboManager.RemoveUboContainer(sUboCameraBlock));
}

} /// ::dy namespace

