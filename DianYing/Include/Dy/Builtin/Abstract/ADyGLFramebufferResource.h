#ifndef GUARD_DY_BUILTIN_ABSTRACT_ADYGLFRAMEBUFFERRRESOURCE_H
#define GUARD_DY_BUILTIN_ABSTRACT_ADYGLFRAMEBUFFERRRESOURCE_H
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

#include <Dy/Builtin/Interface/IDyResource.h>
#include <Dy/Meta/Type/EDyResourceType.h>
#include <Dy/Meta/Information/MetaInfoFrameBuffer.h>

namespace dy
{

struct ADyGLFrameBufferResource : public IDyResource
{
protected:
  /// Temporary frame buffer object meta info.
  PDyGlFrameBufferInstanceMetaInfo mMetaInfo{};
public:
  static constexpr auto value = EDyResourceType::GLFrameBuffer;

  /// @brief Get meta information that has frame buffer meta information.
  std::any GetMetaInfo() override final;
};

} /// ::dy namespace

#endif /// GUARD_DY_BUILTIN_ABSTRACT_ADYGLFRAMEBUFFERRRESOURCE_H