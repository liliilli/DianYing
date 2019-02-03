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
#include <Dy/Component/CDySoundSource.h>

namespace dy
{

EDySuccess CDySoundSource::Initialize(_MIN_ const PDySoundSourceComponentMetaInfo& descriptor)
{
  //MDY_UNEXPECTED_BRANCH();

  if (descriptor.mInitiallyActivated == true) { this->Activate(); }
  return DY_FAILURE;
}

void CDySoundSource::Release()
{
  if (this->IsComponentActivated() == true) { this->Deactivate(); }
}

void CDySoundSource::Update(_MIN_ TF32 dt)
{

}

std::string CDySoundSource::ToString()
{
  MDY_UNEXPECTED_BRANCH();
  return "";
}

void CDySoundSource::TryActivateInstance()
{

}

void CDySoundSource::TryDeactivateInstance()
{

}

} /// ::dy namespace