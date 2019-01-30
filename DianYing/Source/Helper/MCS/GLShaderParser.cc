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

/// @brief Search `iNeedle` word in `iBuffer` begin and end range.
/// If not found, just return nullopt (no-value).

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

MDY_SET_IMMUTABLE_STRING(Buffer_Input_UboCamera, R"dy(
layout(std140, binding = 0) uniform CameraBlock
{
  uniform mat4 mProjMatrix;
  uniform mat4 mViewMatrix;
} uCamera;
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
  MDY_ASSERT_FORCE(optMatchedKeywordList.has_value() == true, "Undefined Dy shader module. Failed to load shader.");

  // Check module keyword. If not found anything, output error.
  // If found any module, just replace `#import phrase` with given string to buffer.
  for (auto& keyword : optMatchedKeywordList.value())
  {
    switch (DyStrSwitchInput(keyword))
    {
    case DyStrCase("Input_DefaultVao"): exportShaderBuffer += Buffer_Input_DefaultVao; break;
    case DyStrCase("Input_UboCamera"):  exportShaderBuffer += Buffer_Input_UboCamera; break;
    default: MDY_NOT_IMPLEMENTED_ASSERT(); break;
    }
  }

  // Append next buffer.
  exportShaderBuffer += optNext.has_value() == true ? ParseGLShader(optNext.value()) : "";
  // Return.
  return exportShaderBuffer;
}

} /// ::dy::mcs namespace