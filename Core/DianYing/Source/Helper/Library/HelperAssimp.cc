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

#ifdef false
/// Header file
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <Dy/Management/LoggingManager.h>

namespace dy
{

std::optional<std::pair<std::shared_ptr<Assimp::Importer>, NotNull<const aiScene*>>>
ReadModel(_MIN_ const std::string& iFilePath)
{
  /// Created `aiScene` will be removed automatically when smtptr of Importer ref counting is 0.
  auto ptrsmtImporter = std::make_shared<Assimp::Importer>();
  
  const aiScene* ptrModelScene = ptrsmtImporter->ReadFile(iFilePath,
      aiProcess_Triangulate 
    | aiProcess_GenNormals
    | aiProcess_GenUVCoords);
  if (MDY_CHECK_ISNULL(ptrModelScene))
  { // If failed to load model, just return no-value so have it processed from outside.
    DyPushLogDebugCritical("Failed to read model file, %s.", iFilePath);
    return std::nullopt;
  }
  else
  {
    if (ptrModelScene->mRootNode == nullptr 
    ||  MDY_BITMASK_FLAG_TRUE(ptrModelScene->mFlags, AI_SCENE_FLAGS_INCOMPLETE))
    {
      DyPushLogDebugCritical("Failed to read model file, %s.", iFilePath);
      return std::nullopt;
    }

    return std::make_pair(ptrsmtImporter, DyMakeNotNull(ptrModelScene));
  }
}

} /// ::dy namespace
#endif