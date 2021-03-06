#ifndef GUARD_DY_HELPER_MCS_GLShaderParser_H
#define GUARD_DY_HELPER_MCS_GLShaderParser_H
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

namespace dy::mcs
{

/// @struct DParsingArgs
/// @brief Arguments instance for parsing shader.
struct DParsingArgs final
{
  const std::string* mShaderString = nullptr;
  bool mIsInstantiable = false;
};

/// @brief Parse variant opengl shader string, return parsed buffer to read properly in OpenGL library.
MDY_NODISCARD std::string ParseGLShader(const DParsingArgs& iShaderString);

} /// ::dy::mcs namespace

#endif /// GUARD_DY_HELPER_MCS_GLShaderParser_H