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

/// Header file
#include <Dy/Builtin/Prefab/FBtPbCameraMan.h>

namespace dy::builtin
{

FDyBtPbCameraMan::FDyBtPbCameraMan()
{
  this->mJsonPrefab = 
R"dy(
{
  "Name": "DyCameraMan", "Type": "Actor",
  "CommonProperties": { "ParentSpecifierName": "", "ObjectTag": "" },
  "ComponentList": [
    { "Type": "Camera", "Activated": true,
      "Details": {
        "FieldOfView": 70.0, 
        "Projection": "Perspective", 
        "ClippingNear": 0.1, "ClippingFar": 600,
        "ViewportRect": { "LeftDown": { "X": 0, "Y": 0 }, "RightUp": { "X": 1, "Y": 1 } },
        "IsFocusInstantly": true, "IsMeshUnclipped": false, "Is3DListener": false
      }
    }
  ]
}
)dy";

}

} /// ::dy::builtin namespace