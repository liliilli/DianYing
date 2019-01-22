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
#include "HelperAssimp.h"
#include <assimp/scene.h>
#include <assimp/postprocess.h>

std::optional<std::pair<std::shared_ptr<Assimp::Importer>, NotNull<const aiScene*>>>
ReadModel(const std::string& iFilePath)
{
  /// Created `aiScene` will be removed automatically when smtptr of Importer ref counting is 0.
  auto ptrsmtImporter = std::make_shared<Assimp::Importer>();
  
  const aiScene* ptrModelScene = ptrsmtImporter->ReadFile(iFilePath,
      aiProcess_Triangulate 
    | aiProcess_GenNormals
    | aiProcess_GenUVCoords);
  if (ptrModelScene == nullptr) { return std::nullopt; }
  else
  {
    if (ptrModelScene->mRootNode == nullptr 
    ||  (ptrModelScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) != 0) { return std::nullopt; }

    return std::make_pair(ptrsmtImporter, DyMakeNotNull(ptrModelScene));
  }
}
