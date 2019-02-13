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
#include "Data_Material.h"

void to_json(nlohmann::json& j, const DMaterial& p)
{
  j = nlohmann::json
  {
    {"TextureSpecifierList", p.mTextureSpecifierList},
    p.mBlendMode
  };
}

void from_json(const nlohmann::json& j, DMaterial& p)
{
  (void)j;
  (void)p;
}

void to_json(nlohmann::json& j, const DMaterial::DTexture& p)
{
  j = nlohmann::json
  {
    {"Specifier", p.mTextureSpecifier},
    p.mTextureMapType
  };
}

void from_json(const nlohmann::json& j, DMaterial::DTexture& p)
{
  (void)j;
  (void)p;
}
