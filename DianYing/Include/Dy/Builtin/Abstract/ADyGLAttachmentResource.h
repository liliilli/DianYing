#ifndef GUARD_DY_BUILTIN_ABSTRACT_ADYGLATTACHMENTRESOURCE_H
#define GUARD_DY_BUILTIN_ABSTRACT_ADYGLATTACHMENTRESOURCE_H
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

#include <Dy/Builtin/Interface/IDyResource.h>
#include <Dy/Meta/Information/MetaInfoAttachment.h>
#include <Dy/Meta/Type/EDyResourceType.h>

namespace dy
{

struct ADyGLAttachmentResource : public IDyResource
{
protected:
  /// Temporary attachment meta info.
  PDyGlAttachmentInstanceMetaInfo mAttachmentMetaInfo{};
public:
  static constexpr auto value = EDyResourceType::GLAttachment;

  /// @brief Get meta information that has buffer to texture and properties.
  std::any GetMetaInfo() override final;
};

} /// ::dy namespace

#endif /// GUARD_DY_BUILTIN_ABSTRACT_ADYGLATTACHMENTRESOURCE_H