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
#include <Dy/Builtin/Abstract/ATextureResource.h>
#include <Dy/Helper/System/Assertion.h>
#include <Dy/Meta/Information/MetaInfoTexture.h>

namespace dy
{

std::any ATextureResource::GetMetaInfo()
{
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(this->mPtrBuffer), "Builtin texture buffer must be valid.");

  this->mTextureMetaInfo.mPtrBuiltinBuffer  = this->mPtrBuffer;
  this->mTextureMetaInfo.mSourceType        = EDyResourceSource::Builtin;
  return this->mTextureMetaInfo;
}

} /// ::dy namespace