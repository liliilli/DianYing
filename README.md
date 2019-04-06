# Dy (DianYing)

| Appveyor | Codacy |
| ---- | ---- |
| [![Build status](https://ci.appveyor.com/api/projects/status/8bgpu922enlkqfac?svg=true)](https://ci.appveyor.com/project/liliilli/dianying) | [![Codacy Badge](https://api.codacy.com/project/badge/Grade/88783a7a1a724c88bc106f601b6b2a31)](https://www.codacy.com/app/liliilli/DianYing?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=liliilli/DianYing&amp;utm_campaign=Badge_Grade) |


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

Model Exporter tool. This tool will be deprecated soon and replaced with `DyModelExporter` GUI tool.

## Log

2019-04-03 Replace platform build tool settings v141 with v142. (Visual Studio 2019)

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
DyModelExporter uses Qt open-source edition library as dynamic library.

https://doc.qt.io/qt-5/lgpl.html