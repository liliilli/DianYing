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
#include "../../Include/Data/Data_MeshData.h"
#include <nlohmann/json.hpp>

void to_json(nlohmann::json& j, const DDyVertexInformation& p)
{
  j = nlohmann::json
  {
    {"ve", p.mPosition}, 
    {"nm", p.mNormal}, 
    {"t0", p.mTexCoords0},
    {"t1", p.mTexCoords1},
    {"ta", p.mTangent},
    {"bt", p.mBitangent},
    {"bo", p.mBoneId},
    {"wg", p.mWeights}
  };
}

void from_json(const nlohmann::json& j, DDyVertexInformation& p)
{
  (void)j;
  (void)p;
}

void to_json(nlohmann::json& j, const DMesh& p)
{
  j = nlohmann::json
  {
    {"vl", p.mVertexList}, 
    {"il", p.mIndexList}, 
  };
}

void from_json(const nlohmann::json& j, DMesh& p)
{
  (void)j;
  (void)p;
}
