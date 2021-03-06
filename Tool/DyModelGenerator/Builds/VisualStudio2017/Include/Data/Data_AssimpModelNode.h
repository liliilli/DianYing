#pragma once
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

#include <vector>
#include "../Library/HelperPointer.h"

struct aiNode;

/// @struct Data_AssimpModelNode
/// @brief Assimp mode node for processing internally in this tool.
struct Data_AssimpModelNode final
{
public:
  Data_AssimpModelNode(NotNull<const aiNode*> iPtrNode);

  NotNull<const aiNode*> mPtrNode;
  std::vector<Data_AssimpModelNode> mChildNodeList;
};
