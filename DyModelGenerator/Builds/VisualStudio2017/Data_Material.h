#pragma once
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

#include <vector>
#include "Enum_MaterialType.h"

struct DMaterial final
{
  struct DTexture final
  {
    std::string       mTextureSpecifier;
    EDyTextureMapType mTextureMapType;
  };

  std::vector<DTexture>  mTextureSpecifierList;
  EDyMaterialBlendMode   mBlendMode;
};

void to_json(nlohmann::json& j, const DMaterial& p);
void from_json(const nlohmann::json& j, DMaterial& p);

void to_json(nlohmann::json& j, const DMaterial::DTexture& p);
void from_json(const nlohmann::json& j, DMaterial::DTexture& p);
