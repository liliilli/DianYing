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
#include <Dy/Meta/Information/ModelMetaInformation.h>
#include <Dy/Helper/Library/HelperJson.h>

//!
//! Forward declaration
//!

namespace
{



} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy
{

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyModelInstanceMetaInfo_Deprecated& p)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyModelInstanceMetaInfo_Deprecated& p)
{
  /* Template
   * "GLModelSpecifierName":
     {
       "SpecifierName": "Name",
       "ModelPath": "C:/Path"
     }
   */

  p.mSourceType = EDyResourceSource::External;
  p.mSpecifierName = DyJsonGetValueFrom<std::string>(j, "SpecifierName");
  p.mExternalModelPath  = DyJsonGetValueFrom<std::string>(j, "ModelPath");
}

} /// ::dy namespace