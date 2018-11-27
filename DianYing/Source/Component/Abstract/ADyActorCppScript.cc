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
#include <Dy/Component/Abstract/ADyActorCppScript.h>
#include <Dy/Component/Internal/CDyActorScriptCpp.h>

namespace dy
{

FDyActor& ADyActorCppScript::GetActorReference()
{
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(this->mOutside), "Unexpected error occurred.");
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(this->mOutside->GetBindedActor()), "Unexpected error occurred.");
  return *this->mOutside->GetBindedActor();
}

} /// ::dy namespace