#include <precompiled.h>
///
/// MIT License
/// Copyright (c) 2018 Jongmin Yun
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
#include <Dy/Helper/Type/ThreeStateBool.h>

namespace dy
{

DDy3StateBool::DDy3StateBool() : mParent(false), mInput(false), mOutput(false)
{
  this->mOldOutput = this->mOutput;
}

DDy3StateBool::DDy3StateBool(const bool parent, const bool input) : mParent(parent), mInput(input), mOutput(parent && input)
{
  this->mOldOutput = this->mOutput;
}

void DDy3StateBool::UpdateInput(const bool newInput) noexcept
{
  this->mInput = newInput;
  this->pUpdateOutputs();
}

void DDy3StateBool::UpdateParent(const bool newParent) noexcept
{
  this->mParent = newParent;
  this->pUpdateOutputs();
}

bool DDy3StateBool::GetOutput() const noexcept
{
  return this->mOutput;
}

bool DDy3StateBool::IsOutputValueChanged() const noexcept
{
  return this->mOutput != this->mOldOutput;
}

void DDy3StateBool::pUpdateOutputs()
{
  this->mOldOutput  = this->mOutput;
  this->mOutput     = this->mParent && this->mInput;
}

} /// ::dy namespace