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
#include <Dy/Core/Resource/Resource/FDyModelResource.h>
#include <Dy/Core/Resource/Internal/FDyModelVBOIntermediate.h>

namespace dy
{

FDyModelResource::FDyModelResource(_MINOUT_ FDyModelVBOIntermediate& input)
{
  // Intermediate mesh resource list must be mutable, \n
  // so to be transfer properties into actual mesh resource by resetting intermediate instance properties.
  auto& intermediateMeshList = input.GetIntermediateMeshList();
  this->mMeshResource.reserve(intermediateMeshList.size());

  for (auto& intermediateMesh : intermediateMeshList) { mMeshResource.emplace_back(intermediateMesh); }
}

FDyModelResource::~FDyModelResource()
{
  this->mMeshResource.clear();
}

} /// ::dy namespace