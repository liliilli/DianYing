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
#include "Data_AssimpModelNode.h"
#include <assimp/scene.h>

Data_AssimpModelNode::Data_AssimpModelNode(NotNull<const aiNode*> iPtrNode) :
    mPtrNode{iPtrNode}
{
  for (unsigned i = 0; i < iPtrNode->mNumChildren; ++i)
  {
    this->mChildNodeList.emplace_back(DyMakeNotNull(iPtrNode->mChildren[i]));
  }
}
