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
#include <Dy/Meta/Information/GLShaderMetaInformation.h>
#include <DY/Helper/Library/HelperJson.h>

//!
//! Forward declaration
//!

namespace
{

MDY_SET_IMMUTABLE_STRING(sHeader_Vertex,    "Vertex");
MDY_SET_IMMUTABLE_STRING(sHeader_Hull,      "Hull");
MDY_SET_IMMUTABLE_STRING(sHeader_Domain,    "Domain");
MDY_SET_IMMUTABLE_STRING(sHeader_Geometry,  "Geometry");
MDY_SET_IMMUTABLE_STRING(sHeader_Pixel,     "Pixel");
MDY_SET_IMMUTABLE_STRING(sHeader_Compute,   "Compute");

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy
{

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyGLShaderInstanceMetaInfo& p)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyGLShaderInstanceMetaInfo& p)
{
  p.mSourceType = EDyResourceSource::External;
  if (DyCheckHeaderIsExist(j, sHeader_Vertex) == DY_SUCCESS)
  {
    p.GetFragment(EDyShaderFragmentType::Vertex).mExternalFilePath = DyJsonGetValueFrom<std::string>(j, sHeader_Vertex);
  }
  if (DyCheckHeaderIsExist(j, sHeader_Hull) == DY_SUCCESS)
  {
    p.GetFragment(EDyShaderFragmentType::Hull).mExternalFilePath = DyJsonGetValueFrom<std::string>(j, sHeader_Hull);
  }
  if (DyCheckHeaderIsExist(j, sHeader_Domain) == DY_SUCCESS)
  {
    p.GetFragment(EDyShaderFragmentType::Domain).mExternalFilePath = DyJsonGetValueFrom<std::string>(j, sHeader_Domain);
  }
  if (DyCheckHeaderIsExist(j, sHeader_Geometry) == DY_SUCCESS)
  {
    p.GetFragment(EDyShaderFragmentType::Geometry).mExternalFilePath = DyJsonGetValueFrom<std::string>(j, sHeader_Geometry);
  }
  if (DyCheckHeaderIsExist(j, sHeader_Pixel) == DY_SUCCESS)
  {
    p.GetFragment(EDyShaderFragmentType::Pixel).mExternalFilePath = DyJsonGetValueFrom<std::string>(j, sHeader_Pixel);
  }
  if (DyCheckHeaderIsExist(j, sHeader_Compute) == DY_SUCCESS)
  {
    p.GetFragment(EDyShaderFragmentType::Compute).mExternalFilePath = DyJsonGetValueFrom<std::string>(j, sHeader_Compute);
    p.mIsComputeShader = true;
  }
}

} /// ::dy namespace