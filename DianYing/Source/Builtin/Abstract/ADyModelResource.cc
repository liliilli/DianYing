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
#include <Dy/Builtin/Abstract/ADyModelResource.h>
#include <Dy/Meta/Information/ModelMetaInformation.h>

namespace dy
{

std::any ADyModelResource::GetMetaInfo()
{
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(this->mPtrBuffer), "Builtin model buffer must not be nulled.");

  PDyModelInstanceMetaInfo result{};
  result.mSourceType            = EDyResourceSource::Builtin;
  result.mSpecifierName         = this->mPtrBuffer->mModelName;
  result.mPtrBuiltinModelBuffer = this->mPtrBuffer;

  return result;
}


} /// ::dy namespace