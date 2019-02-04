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
#include <Dy/Component/CDyPhysicsRigidbody.h>

namespace dy
{

CDyPhysicsRigidbody::CDyPhysicsRigidbody(FDyActor& actorReference) : 
    ADyGeneralBaseComponent{actorReference}
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

EDySuccess CDyPhysicsRigidbody::Initialize(_MIN_ const PDyRigidbodyComponentMetaInfo& descriptor)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  return DY_FAILURE;
}

void CDyPhysicsRigidbody::Release()
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

std::string CDyPhysicsRigidbody::ToString()
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  return "";
}

void CDyPhysicsRigidbody::TryActivateInstance()
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void CDyPhysicsRigidbody::TryDeactivateInstance()
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

} /// ::dy namespace
