# Dy (DianYing)

DianYing(电影) (A.K.A `Dy`) is Open-sourced OpenGL 3D gaming framework. This framework uses OpenGL as rendering API, PhysX as Physics engine and miscellaneous features to support framework.

## Structure

### ./Common

This directory contains common project and third-party libraries to be used on core & tool projects.

* `DyExpression` : Compile time expression and miscellaneous helper function library.
* `DyMath` : Math library for code reproductibility of Dy projects.

### ./DianYing

Core framework project.

### ./DyFontAtlasGenerator

SDF (Signed Distance Field) Font Atlas generator tool that uses Qt5.

### ./DyModelGenerator

Model Exporter tool.

## Copyright

### MIT

Projects except for DyFontAtlasGenerator uses MIT License.

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

### LPGL v3

DyFontAtlasGenerator uses Qt open-source edition library as dynamic library.

https://doc.qt.io/qt-5/lgpl.html