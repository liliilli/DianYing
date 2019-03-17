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
#include <Dy/Core/Resource/Internal/XGeometryTypes.h>
#include <nlohmann/json.hpp>

namespace dy
{

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ DDefaultVertexInfo& p)
{
  p.mPosition   = j["ve"].get<DVector3>();
  p.mNormal     = j["nm"].get<DVector3>();
  p.mTexCoord0  = j["t0"].get<DVector2>();
}

} /// ::dy namespace