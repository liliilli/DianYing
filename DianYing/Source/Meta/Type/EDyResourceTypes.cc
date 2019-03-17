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
#include <Dy/Meta/Type/EDyResourceTypes.h>
#include <Dy/Helper/Internal/XStringSwitch.h>

namespace dy
{

EDyShaderFragmentType DyConvertStringToEDyShaderFragmentType(_MIN_ const std::string& iString) noexcept
{
  switch (SwitchStrInput(iString))
  {
  case CaseStr("Vert"): return EDyShaderFragmentType::Vertex; 
  case CaseStr("Hull"): return EDyShaderFragmentType::Hull; 
  case CaseStr("Domn"): return EDyShaderFragmentType::Domain; 
  case CaseStr("Geom"): return EDyShaderFragmentType::Geometry; 
  case CaseStr("Pixl"): return EDyShaderFragmentType::Pixel; 
  case CaseStr("Comp"): return EDyShaderFragmentType::Compute; 
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(EDyShaderFragmentType::NoneError);
  }
}

} /// ::dy namespace