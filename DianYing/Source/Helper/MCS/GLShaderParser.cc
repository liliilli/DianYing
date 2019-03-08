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
#include <Dy/Helper/MCS/GLShaderParser.h>

#include <algorithm>
#include <Dy/Helper/Library/HelperRegex.h>
#include <Dy/Helper/StringSwitch.h>

//!
//! Local translation unit function & data
//!

namespace
{

/// @brief Search `iNeedle` word in `iBuffer` string buffer.
/// If not found, just return nullopt (no-value).
MDY_NODISCARD std::optional<std::string::const_iterator> 
DyStrInQuote(_MIN_ const std::string& iBuffer, _MIN_ const std::string& iNeedle)
{
  auto it = std::search(
      MDY_BIND_BEGIN_END(iBuffer), 
      std::boyer_moore_searcher(MDY_BIND_BEGIN_END(iNeedle))
  );

  // If not found, just return nullopt or iterator (const).
  if (it == iBuffer.end())  { return std::nullopt; }
  else                      { return it; }
}

/// @brief Separate string buffer, the line which is in given `iIterator` is medium output.  
/// from buffer start and the previous line will be left and from next line to end is right output.
MDY_NODISCARD std::tuple<std::optional<std::string>, std::optional<std::string>, std::optional<std::string>>
DySeparateBufferTo3Way(_MIN_ const std::string& iBuffer, _MIN_ const std::string::const_iterator& iIterator)
{
  using TOptString = std::optional<std::string>;
  std::tuple<TOptString, TOptString, TOptString> result = {std::nullopt, std::nullopt, std::nullopt};

  // Find previous last '\n'.
  std::string returnCarriage = "\n";
  const auto itPrev = std::find_end(iBuffer.cbegin(), iIterator, returnCarriage.begin(), returnCarriage.end());
  // If find previous '\n', so `iIterator` is not on first line,
  // Extract `previous` buffer with '\n'. (std::distance(iBuffer.cbegin(), itPrev) + 1)
  if (itPrev != iBuffer.cbegin()) 
  {
    std::get<0>(result) = iBuffer.substr(0, std::distance(iBuffer.cbegin(), itPrev) + 1);
  }

  // find next first '\n'.
  const auto itNext = std::find(iIterator, iBuffer.cend(), '\n');
  // If find next '\n', so `iIterator` is not on first line,
  // Extract `next ` buffer with '\n'.
  if (itPrev != iBuffer.cend()) 
  {
    std::get<2>(result) = iBuffer.substr(
        std::distance(iBuffer.cbegin(), itNext) + 1, 
        std::distance(itNext, iBuffer.cend() - 1)
    );
  }

  // and using `itPrev` and `itNext`, insert medium line.
  // std::distance is half-open range [a, a + count).
  const auto midStart = itPrev == iBuffer.cbegin() ? 0 : std::distance(iBuffer.cbegin(), itPrev) + 1;
  const auto midEnd   = std::distance(iBuffer.cbegin() + midStart, itNext);
  std::get<1>(result) = iBuffer.substr(midStart, midEnd + 1);

  return result;
}

} /// ::anonymous namespace

namespace 
{

/// @brief Default vertex array object module.
/// #import <Input_DefaultVao>;
MDY_SET_IMMUTABLE_STRING(Buffer_Input_DefaultVao, R"dy(
layout (location = 0) in vec3 dyPosition;
layout (location = 1) in vec3 dyNormal;
layout (location = 2) in vec2 dyTexCoord0;
layout (location = 3) in vec2 dyTexCoord1;
layout (location = 4) in vec3 dyTangent;
layout (location = 5) in vec3 dyBitangent;
layout (location = 6) in ivec4 dyBoneId;
layout (location = 7) in vec4 dyBoneWeight;
)dy");

/// @brief Uniform buffer object for focused camera block.
/// #import <Input_UboCamera>;
MDY_SET_IMMUTABLE_STRING(Buffer_Input_UboCamera, R"dy(
layout(std140, binding = 0) uniform CameraBlock
{
  uniform mat4 mProjMatrix;
  uniform mat4 mViewMatrix;
} uCamera;

// @brief Get focused camera's PV matrix.
mat4 Dy_GetCameraPv() { return uCamera.mProjMatrix * uCamera.mViewMatrix; }
)dy");

/// @brief Skinned animation input uniform variables.
/// #import <Input_SkinAnimation>;
MDY_SET_IMMUTABLE_STRING(Buffer_Input_SkinAnimation, R"dy(
#define BONE_MAX_COUNT 144
uniform mat4 mBoneTransform[BONE_MAX_COUNT];
uniform int  mNumBoneTransform = 0;

// @brief Requires module <Input_DefaultVao>.
mat4 Dy_GetBoneTransform()
{
  mat4 result = mat4(0);
  if (dyBoneId[0] != -1 && dyBoneId[0] < mNumBoneTransform) { result += mBoneTransform[dyBoneId[0]] * dyBoneWeight[0]; }
  if (dyBoneId[1] != -1 && dyBoneId[1] < mNumBoneTransform) { result += mBoneTransform[dyBoneId[1]] * dyBoneWeight[1]; }
  if (dyBoneId[2] != -1 && dyBoneId[2] < mNumBoneTransform) { result += mBoneTransform[dyBoneId[2]] * dyBoneWeight[2]; }
  if (dyBoneId[3] != -1 && dyBoneId[3] < mNumBoneTransform) { result += mBoneTransform[dyBoneId[3]] * dyBoneWeight[3]; }
  return result;
}
)dy");

/// @brief Default Texture2D input uniform varaibles.
/// #import <Input_DefaultTexture2D>;
MDY_SET_IMMUTABLE_STRING(Buffer_Input_DefaultTexture2D, R"dy(
layout (binding = 0) uniform sampler2D uTexture0;
layout (binding = 1) uniform sampler2D uTexture1;
layout (binding = 2) uniform sampler2D uTexture2;
layout (binding = 3) uniform sampler2D uTexture3;
layout (binding = 4) uniform sampler2D uTexture4;
layout (binding = 5) uniform sampler2D uTexture5;
layout (binding = 6) uniform sampler2D uTexture6;
layout (binding = 7) uniform sampler2D uTexture7;
layout (binding = 8) uniform sampler2D uTexture8;
layout (binding = 9) uniform sampler2D uTexture9;
)dy");

/// @brief Default opaque stream output module.
/// #import <Output_OpaqueStream>;
MDY_SET_IMMUTABLE_STRING(Buffer_Output_OpaqueStream, R"dy(
layout (location = 0) out vec4 gUnlit;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gSpecular;
layout (location = 3) out vec4 gPosition;
layout (location = 4) out vec4 gActorId;

uniform float uBtDyActorId = 0;

void DyBindActorId() { gActorId = vec4(uBtDyActorId, 0, 0, 1); }
)dy");

} /// ::anonymous namespace

//!
//! Implementation
//!

namespace dy::mcs
{

std::string ParseGLShader(_MIN_ const std::string& iShaderString)
{
  // Find `#import `.
  auto optImportIt = DyStrInQuote(iShaderString, "#import ");
  if (optImportIt.has_value() == false) { return iShaderString; }

  // Find proper importing module name, (such as ModuleDefaultVao)
  auto [optPrev, optMid, optNext] = DySeparateBufferTo3Way(iShaderString, optImportIt.value());
  // Setup result (export) shader buffer.
  std::string exportShaderBuffer = optPrev.has_value() == false ? "" : optPrev.value();

  // Get Dy shader module keywords.
  const auto optMatchedKeywordList = DyRegexGetMatchedKeyword(optMid.value(), R"(#import <([\w]+)>;)");
  MDY_ASSERT_MSG_FORCE(optMatchedKeywordList.has_value() == true, "Undefined Dy shader module. Failed to load shader.");

  // Check module keyword. If not found anything, output error.
  // If found any module, just replace `#import phrase` with given string to buffer.
  for (auto& keyword : optMatchedKeywordList.value())
  {
    switch (DyStrSwitchInput(keyword))
    {
    case DyStrCase("Input_DefaultVao"):       exportShaderBuffer += Buffer_Input_DefaultVao; break;
    case DyStrCase("Input_UboCamera"):        exportShaderBuffer += Buffer_Input_UboCamera; break;
    case DyStrCase("Input_SkinAnimation"):    exportShaderBuffer += Buffer_Input_SkinAnimation; break;
    case DyStrCase("Input_DefaultTexture2D"): exportShaderBuffer += Buffer_Input_DefaultTexture2D; break;
    case DyStrCase("Output_OpaqueStream"):    exportShaderBuffer += Buffer_Output_OpaqueStream; break;
    default: MDY_NOT_IMPLEMENTED_ASSERT(); break;
    }
  }

  // Append next buffer.
  exportShaderBuffer += optNext.has_value() == true ? ParseGLShader(optNext.value()) : "";
  // Return.
  return exportShaderBuffer;
}

} /// ::dy::mcs namespace