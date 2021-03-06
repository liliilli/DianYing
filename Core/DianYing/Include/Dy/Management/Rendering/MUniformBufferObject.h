#ifndef GUARD_DY_MANAGEMENT_INTERNAL_UNIFORMBUFFEROBJECTMANAGER_H
#define GUARD_DY_MANAGEMENT_INTERNAL_UNIFORMBUFFEROBJECTMANAGER_H
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

#include <unordered_map>
#include <unordered_set>
#include <Dy/Management/Interface/ISingletonCrtp.h>
#include <Dy/Management/Type/UBOInformation.h>

namespace dy
{

/// @class MUniformBufferObject
/// @brief Uniform buffer object management type.
class MUniformBufferObject final : public ISingleton<MUniformBufferObject>
{
  MDY_SINGLETON_PROPERTIES(MUniformBufferObject);
  MDY_SINGLETON_DERIVED(MUniformBufferObject);

public:
  ///
  /// @brief  Create ubo container buffer resource.
  /// @param  descriptor Descriptor instance for creating UBO instance.
  /// @return If succeeded, return true but false.
  ///
  MDY_NODISCARD EDySuccess CreateUboContainer(_MIN_ const PDyUboConstructionDescriptor& descriptor);

  ///
  /// @brief  Get valid and immutable ubo resource container.
  /// Returned ptr must not be released cv-qualifier using const_cast.
  /// @param  specifier UBO specifier name to access.
  /// @return If succeeded, return valid instance pointer but nullptr.
  ///
  MDY_NODISCARD const DDyUboInstanceInformation* GetUboContainer(_MIN_ const std::string& specifier);

  /// @brief  Update UBO resource buffer with valid buffer instance and pointer.
  /// @param  specifier
  /// @param  bufferStartByte
  /// @param  bufferWrapSize
  /// @param  bufferCopyPtr
  /// @return If succeeded return EDySuccess::DY_SUCCESS or EDySuccess::DY_FAILURE;
  EDySuccess UpdateUboContainer(
    const std::string& specifier,
    TU32 bufferStartByte,
    TU32 bufferWrapSize,
    const void* bufferCopyPtr);

  /// @brief  Clear UBO resource buffer with start byte pointer and wrapping byte size.
  /// @param  specifier
  /// @param  bufferStartByte
  /// @param  bufferWrapSize
  /// @return
  EDySuccess ClearUboContainer(
    const std::string& specifier,
    TU32 bufferStartByte,
    TU32 bufferWrapSize);

  ///
  /// @brief  Remove UBO resource buffer with valid specifier name.
  /// @param  specifier
  /// @return If succeeded to remove UBO resource return EDySuccess::DY_SUCCESS or EDySuccess::DY_FAILURE.
  ///
  MDY_NODISCARD EDySuccess RemoveUboContainer(_MIN_ const std::string& specifier);

private:
  /// Valid UBO instace container map.
  std::unordered_map<std::string, std::unique_ptr<DDyUboInstanceInformation>> mUboMap = {};
  ///
  std::unordered_set<TU32> mBeingUsedUboBufferIndexSet = {};
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_INTERNAL_UNIFORMBUFFEROBJECTMANAGER_H