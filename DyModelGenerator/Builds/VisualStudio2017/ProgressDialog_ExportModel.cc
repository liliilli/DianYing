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
#include "ProgressDialog_ExportModel.h"
#include "HelperConstant.h"
#include "HelperMacroes.h"

ProgressDialog_ExportModel::ProgressDialog_ExportModel(MDY_NOTUSED EExportFlags iFlags) :
    ThreadWithProgressWindow("busy doing some important things...", true, true)
{

}

void ProgressDialog_ExportModel::run()
{
  setProgress(kDialog_SpinningBar); 
  setStatusMessage("Exporting..."); 
  /*
   * vec3 pos, vec3 normal, vec3 tangent, vec3 bitangent, vec2 texcoord0 (st만), boneId vec4, weight vec4, 
   * 본 오프셋 매트릭스, *  해당 본의 파이널 트랜스폼. 
   * 뒤쪽 두개 요소는 메쉬별로 나누면 될 것 같고, 앞의 요소들은 버텍스마다 해서 시리얼라이즈 해줘야겠네 ㅠ
   */

  wait(500);
}

void ProgressDialog_ExportModel::threadComplete(MDY_NOTUSED bool userPressedCancel)
{
  delete this;
}
