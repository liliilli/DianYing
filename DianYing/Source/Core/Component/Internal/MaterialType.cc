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

#include <Dy/Core/Component/Internal/MaterialType.h>
#include <Dy/Management/IO/IOResourceManager.h>

namespace dy
{

std::string PDyMaterialConstructionDescriptor::ToString()
{
  auto log = fmt::format(
R"dy(PDyMaterialConstructionDescriptor
Material Name : {0}
Shader Name : {1}
)dy", this->mMaterialName, this->mShaderName);

  for (const auto& textureName : this->mTextureNames)
  {
    log += textureName;
  }

  return log;
}

DDyMaterialShaderTuple::DDyMaterialShaderTuple(const std::string& shaderName) :
    mShaderName(shaderName),
    mValidShaderPointer(MDyIOResource::GetInstance().GetShaderResource(shaderName))
{ }

DDyMaterialTextureTuple::DDyMaterialTextureTuple(const std::string& textureName) :
    mTextureName(textureName),
    mValidTexturePointer(MDyIOResource::GetInstance().GetTextureResource(textureName))
{ }

} /// ::dy namespace