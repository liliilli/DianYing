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
#include <Dy/Management/Helper/PhysXErrorCallback.h>
#include <Dy/Management/LoggingManager.h>

namespace dy
{

void FDyPhysXErrorCallback::reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line)
{
  static constexpr const char* logTemplate = "PhysX | {} | {} | {} Line. {}";

  switch (code)
  {
  default: break;
  case physx::PxErrorCode::eDEBUG_INFO:
    MDY_LOG_INFO_D(logTemplate,   MDY_TO_STRING(physx::PxErrorCode::eDEBUG_INFO), message, file, line);
    break;
  case physx::PxErrorCode::eDEBUG_WARNING:
    MDY_LOG_WARNING(logTemplate,  MDY_TO_STRING(physx::PxErrorCode::eDEBUG_WARNING), message, file, line);
    break;
  case physx::PxErrorCode::eINVALID_PARAMETER:
    MDY_LOG_ERROR(logTemplate,    MDY_TO_STRING(physx::PxErrorCode::eINVALID_PARAMETER), message, file, line);
    break;
  case physx::PxErrorCode::eINVALID_OPERATION:
    MDY_LOG_ERROR(logTemplate,    MDY_TO_STRING(physx::PxErrorCode::eINVALID_OPERATION), message, file, line);
    break;
  case physx::PxErrorCode::eOUT_OF_MEMORY:
    MDY_LOG_CRITICAL(logTemplate, MDY_TO_STRING(physx::PxErrorCode::eOUT_OF_MEMORY), message, file, line);
    break;
  case physx::PxErrorCode::eINTERNAL_ERROR:
    MDY_LOG_CRITICAL(logTemplate, MDY_TO_STRING(physx::PxErrorCode::eINTERNAL_ERROR), message, file, line);
    break;
  case physx::PxErrorCode::eABORT:
    MDY_LOG_CRITICAL(logTemplate, MDY_TO_STRING(physx::PxErrorCode::eABORT), message, file, line);
    break;
  case physx::PxErrorCode::ePERF_WARNING:
    MDY_LOG_WARNING(logTemplate,  MDY_TO_STRING(physx::PxErrorCode::ePERF_WARNING), message, file, line);
    break;
  }
}

} /// ::dy namespace