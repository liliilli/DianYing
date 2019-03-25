## Dy(DianYing) Common Math Library

Dy(DianYing) Common Math library (A.K.A. `DyMath`) is math library that is used by `Dy` projects for code reproduction.

Other third-party projects can use this library following MIT License.

## Usage

This library is header-only library. Just add the path of include files into your project.

## Features

This math library suports below things.

* Fixed vector `DVector` types, `DVector2` `DVector3` `DVector4` with value types as template parameter.
* Fixed squared matrix `DMatrix` types `DVector2` `DVector3` `DVector4`,
  that only support real type (`TF32` as float, `TF64` as double.)
  * Matrix types are column-major type for compatibility `glsl` and `SPIR-V`.
* 2D 3D Area type that has start point and width `x`, height `y` (and depth as `z` when using 3D area),
  `DArea2D` and `DArea3D`, with value types as template parameter.
* Plane type that has normal and d value, as $$ ax + by + cz + d = 0 $$ and $$ |(a, b, c)| == 1 $$, `DPlane`.
  That only support real type `TF32 ` and `TF64`.
* Clamping type that has compile time range `TStart` and `TEnd`, and supporting compile type. `DClamp`
* and, miscellaneous helper math functions...

Implemented container type's arithmethic operators and functions are automatically proceeded by converting to more coverable type following value category description.

### Value Category

Dy(DianYing) Common Math (a.k.a `DyMath`) has value category to support automatic type conversion between two different types of same container type.

`DyMath` has below types and categories.

| Types  | Description                                          | Category  |
| ------ | ---------------------------------------------------- | --------- |
| TWchr  | Wide character type (16bit or 32bit dependent on OS) | Character |
| TChr8  | 8-bit character type                                 | Character |
| TChr16 | 16-bit character type                                | Character |
| TChr32 | 32-bit character type                                | Character |
| TI8    | 8-bit Signed integer type                            | Signed    |
| TI16   | 16-bit Signed integer type                           | Signed    |
| TI32   | 32-bit Signed integer type                           | Signed    |
| TI64   | 64-bit Signed integer type                           | Signed    |
| TU8    | 8-bit Unsigned integer type                          | Unsigned  |
| TU16   | 16-bit Unsignedinteger type                          | Unsigned  |
| TU32   | 32-bit Unsignedinteger type                          | Unsigned  |
| TU64   | 64-bit Unsignedinteger type                          | Unsigned  |
| TF32   | 32-bit Real type                                     | Real      |
| TF64   | 64-bit Real type                                     | Real      |

* Types in each category are automatically convertible to more-bit types of same category.
* Except for `Character` category, all of category types are convertible to `Real` category type.
* `Signed` and `Unsigned` category types are not convertible to each other because of value coverage.

## Log



## Copyright

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