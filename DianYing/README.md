## DyCore

https://github.com/liliilli/DianYing/tree/trunk/DianYing

`DyCore` is core project for constructing and building `Dy` framework.

## Usage



## Feature

* Various C++ Scripting Features
  * Actor Script
  * Application Global Script
  * Widget Script
* Parallel IO Resource Loading System
* Modulized Rendering Pipeline System
* Rendering using OpenGL 4.3 API
* Builtin Resource Registration System
* Physics System using PhysX 4.0
* Signed Distance Font Text Rendering
* Cascaded Shadow Mapping Feature
* Screen Space Ambient Object Feature
* Weight-Blended OIT Feature
* Loading Model as `dyMesh`

## Todo

| Date     | Category | Description                                      | Status |
| -------- | -------- | ------------------------------------------------ | ------ |
| 19-03-23 |          | Implement dual-depth peeling (Transparency)      | ❌      |
| 19-03-23 |          | Fix opengl crash on other pc environment         | ❌      |
| 19-03-23 |          | Fix CPU Frustum culling malfunction              | ❌      |
| 19-03-23 |          | Improve stability of IO Resource Loading system. | ❌      |
| 19-03-23 | R        | Apply initialtransfrom to model resource.        | ❌      |
| 19-03-23 | R        | Fix model skinned animation.        | ❌      |

## Log

2019-04-02 : Fixed intermittent live-lock bug of IO Resource Loading.


## Copyright

 MIT License
 Copyright (c) 2018-2019 Jongmin Yun

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
