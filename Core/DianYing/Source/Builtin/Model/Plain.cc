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
#include <Dy/Builtin/Model/Plain.h>
#include <Dy/Builtin/Mesh/FDyBtMsPlain.h>
#include <Dy/Builtin/Material/BtMtCheckerWorldPos.h>

namespace dy::builtin
{

void FDyBuiltinModelPlain::ConstructBuffer(PDyModelInstanceMetaInfo& buffer) noexcept
{
  buffer.mSourceType          = EDyResourceSource::Builtin;
  buffer.mSpecifierName       = sName;
  buffer.mMeshList.emplace_back((FDyBtMsPlain::sName), (FDyBtMtCheckerWorldPos::sName));
}

} /// ::dy::builtin namespace