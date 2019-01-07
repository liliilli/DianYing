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
#include <Dy/Builtin/Model/Box.h>
#include <Dy/Builtin/Mesh/FDyBtMsBox.h>

namespace dy::builtin
{

void FDyBuiltinModelBox::ConstructBuffer(PDyModelInstanceMetaInfo& buffer) noexcept
{
  buffer.mSourceType          = EDyResourceSource::Builtin;
  buffer.mSpecifierName       = sName;
  buffer.mIsUsingBuiltinMesh  = true;
  buffer.mBuiltinMeshSpecifierList.emplace_back(FDyBtMsBox::sName);
}

} /// ::dy::builtin namespace