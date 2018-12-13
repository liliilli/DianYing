#ifndef GUARD_DY_CORE_RESOURCE_RESOURCE_FDYMODELRESOURCE_H
#define GUARD_DY_CORE_RESOURCE_RESOURCE_FDYMODELRESOURCE_H
#include "FDyMeshResource.h"

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

//!
//! Forward declaration
//!

namespace dy
{
class FDyModelVBOIntermediate;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

///
/// @class FDyModelResource
/// @brief Model resource class that serve heap instance.
///
class FDyModelResource final
{
public:
  FDyModelResource(_MINOUT_ FDyModelVBOIntermediate& input);
  ~FDyModelResource();

private:
  std::vector<FDyMeshResouce> mMeshResource = {};
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_RESOURCE_FDYMODELRESOURCE_H