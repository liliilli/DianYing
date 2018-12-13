#ifndef GUARD_DY_CORE_RESOURCE_RESOURCE_FDYMESHRESOURCE_H
#define GUARD_DY_CORE_RESOURCE_RESOURCE_FDYMESHRESOURCE_H
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

#include <Dy/Core/Resource/Information/FDyModelInformation.h>

//!
//! Forward declaration
//!

namespace dy
{
class FDyMeshVBOIntermediate;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

///
/// @class FDyMeshResouce
/// @brief
///
class FDyMeshResouce final
{
public:
  /// @brief
  FDyMeshResouce(_MINOUT_ FDyMeshVBOIntermediate& intermediateInstance);
  ~FDyMeshResouce();

private:
  DDyGlBufferIdInformation        mBufferIdInformation     = {};
  DDySubmeshFlagInformation       mMeshFlagInformation     = {};
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_RESOURCE_FDYMESHRESOURCE_H