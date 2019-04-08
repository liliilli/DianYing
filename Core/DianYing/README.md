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
 
### PhysX License
 
PhysX License

Copyright (c) 2018 NVIDIA Corporation. All rights reserved. Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
    Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
    Neither the name of NVIDIA CORPORATION nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
