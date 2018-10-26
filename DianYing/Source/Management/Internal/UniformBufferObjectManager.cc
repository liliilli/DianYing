#include <precompiled.h>
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

/// Header file
#include <Dy/Management/Internal/UniformBufferObjectManager.h>
#include <Dy/Management/LoggingManager.h>

//!
//! Forward declaration & Local translation unit data.
//!

namespace dy
{

MDY_SET_IMMUTABLE_STRING(sFunc_CreateUboContainer, "MDyUniformBufferObject::CreateUboContainer");

} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

EDySuccess MDyUniformBufferObject::pfInitialize()
{ // Do nothing.
  return DY_SUCCESS;
}

EDySuccess MDyUniformBufferObject::pfRelease()
{ // Release all UBO buffer before release management instance.
  for (auto it = this->mUboMap.begin(); it != this->mUboMap.end(); ++it)
  {
    const DDyUboInstanceInformation& instance = *it->second;
    const TU32 bufferIndex  = instance.GetBufferInternalIndex();
    const TU32 bindingIndex = instance.mBindingIndex;
    glDeleteBuffers(1, &bufferIndex);
    it = this->mUboMap.erase(it);
    this->mBeingUsedUboBufferIndexSet.erase(bindingIndex);
  }

  // Make uniform buffer object buffer space
  return DY_SUCCESS;
}

EDySuccess MDyUniformBufferObject::CreateUboContainer(_MIN_ const PDyUboConstructionDescriptor& descriptor)
{ // Duplication check
  if (MDY_CHECK_ISNOTNULL(this->GetUboContainer(descriptor.mUboSpecifierName)))
  {
    MDY_LOG_ERROR("{} | Failed to create UBO container. Name is duplicated. | Name : {}",
                  MSVSTR(sFunc_CreateUboContainer),
                  descriptor.mUboSpecifierName);
    return DY_FAILURE;
  }

  // Binding index duplication check
  if (this->mBeingUsedUboBufferIndexSet.find(descriptor.mBindingIndex) !=
      this->mBeingUsedUboBufferIndexSet.end())
  {
    MDY_LOG_ERROR("{} | Failed to create UBO container. Binding Index is duplicated. | Binding Index : {}",
                  MSVSTR(sFunc_CreateUboContainer),
                  descriptor.mBindingIndex);
    return DY_FAILURE;
  }

  // Check array size and element byte size is not specified correctly.
  if (descriptor.mUboArraySize <= 0)    { return DY_FAILURE; }
  if (descriptor.mUboElementSize <= 0)  { return DY_FAILURE; }

  // GL INTERNAL SPECIFICATION COMPARISON INTEGRITY TEST

  // ~~~ AFTER THAT.... CREATE AND ISSUE BINDING ID.
  auto [it, result] = this->mUboMap.try_emplace(descriptor.mUboSpecifierName, nullptr);
  MDY_ASSERT(result == true, "Unexpected error occurred.");

  auto uboInstance = std::make_unique<DDyUboInstanceInformation>(descriptor);
  glGenBuffers(1, &uboInstance->mBufferIndex);
  glBindBuffer(GL_UNIFORM_BUFFER, uboInstance->mBufferIndex);

  // Copy buffer data or not just creating buffer space.
  void* startAddr = nullptr;
  if (MDY_CHECK_ISNOTNULL(descriptor.mBufferStartPtr)) { startAddr = descriptor.mBufferStartPtr; }

  glBufferData(GL_UNIFORM_BUFFER,
               uboInstance->mContainerSize, startAddr,
               DyGetBufferDrawTypeValue(uboInstance->mBufferDrawType));
  glBindBufferBase(GL_UNIFORM_BUFFER, uboInstance->mBindingIndex, uboInstance->mBufferIndex);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  // Insert container and enroll binding index number.
  this->mBeingUsedUboBufferIndexSet.emplace(uboInstance->mBindingIndex);
  it->second.swap(uboInstance);
  return DY_SUCCESS;
}

const DDyUboInstanceInformation* MDyUniformBufferObject::GetUboContainer(_MIN_ const std::string& specifier)
{
  if (auto it = this->mUboMap.find(specifier); it != this->mUboMap.end()) { return it->second.get(); }
  else                                                                    { return nullptr; }
}

EDySuccess MDyUniformBufferObject::UpdateUboContainer(
    _MIN_ const std::string& specifier,
    _MIN_ const TU32 bufferStartByte,
    _MIN_ const TU32 bufferWrapSize,
    _MIN_ const void* bufferCopyPtr)
{ // Integrity test :: unvalid specifier test
  const DDyUboInstanceInformation* uboPtr = this->GetUboContainer(specifier);
  if (MDY_CHECK_ISNULL(uboPtr))
  {
    MDY_LOG_ERROR("{} | Failed to update UBO container. Given name is not exist. | Name : {}",
        MSVSTR(sFunc_CreateUboContainer),
        specifier);
    return DY_FAILURE;
  }
  // Integrity Test :: OOB Test
  if (bufferStartByte + bufferWrapSize > uboPtr->GetContainerSize())
  {
    MDY_LOG_ERROR("{} | Failed to update UBO container. Buffer size out of bound detected. | {} + {} > {}",
        MSVSTR(sFunc_CreateUboContainer),
        bufferStartByte, bufferWrapSize,
        uboPtr->GetContainerSize());
    return DY_FAILURE;
  }

  // Copy buffer data with [bufferStartByte, bufferWrapSize + bufferStartByte).
  glBindBuffer    (GL_UNIFORM_BUFFER, uboPtr->GetBufferInternalIndex());
  glBufferSubData (GL_UNIFORM_BUFFER, bufferStartByte, bufferWrapSize, bufferCopyPtr);
  glBindBuffer    (GL_UNIFORM_BUFFER, 0);
  return DY_SUCCESS;
}

EDySuccess MDyUniformBufferObject::ClearUboContainer(
    _MIN_ const std::string& specifier,
    _MIN_ const TU32 bufferStartByte,
    _MIN_ const TU32 bufferWrapSize)
{ // Integrity test :: unvalid specifier test
  const DDyUboInstanceInformation* uboPtr = this->GetUboContainer(specifier);
  if (MDY_CHECK_ISNULL(uboPtr))
  {
    MDY_LOG_ERROR("{} | Failed to clear UBO container. Given name is not exist. | Name : {}", MSVSTR(sFunc_CreateUboContainer), specifier);
    return DY_FAILURE;
  }
  // Integrity Test :: OOB Test
  if (bufferStartByte + bufferWrapSize > uboPtr->GetContainerSize())
  {
    MDY_LOG_ERROR("{} | Failed to clear UBO container. Buffer size out of bound detected. | {} + {} > {}",
        MSVSTR(sFunc_CreateUboContainer),
        bufferStartByte, bufferWrapSize,
        uboPtr->GetContainerSize());
    return DY_FAILURE;
  }

  // Clear buffer with 0.
  static constexpr float nullValue[1] = {0.f};
  glBindBuffer(GL_UNIFORM_BUFFER, uboPtr->GetBufferInternalIndex());
  glClearBufferSubData(GL_UNIFORM_BUFFER, GL_RGBA, bufferStartByte, bufferWrapSize, GL_RED, GL_FLOAT, &nullValue);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
  return DY_SUCCESS;
}

EDySuccess MDyUniformBufferObject::RemoveUboContainer(_MIN_ const std::string& specifier)
{ // Integrity test :: unvalid specifier test
  const DDyUboInstanceInformation* uboPtr = this->GetUboContainer(specifier);
  if (MDY_CHECK_ISNULL(uboPtr))
  {
    MDY_LOG_ERROR("{} | Failed to clear UBO container. Given name is not exist. | Name : {}",
        MSVSTR(sFunc_CreateUboContainer),
        specifier);
    return DY_FAILURE;
  }

  // Delete logic
  glDeleteBuffers(1, &uboPtr->mBufferIndex);
  this->mBeingUsedUboBufferIndexSet.erase(uboPtr->mBindingIndex);
  this->mUboMap.erase(specifier);
  return DY_SUCCESS;
}

} /// ::dy namespace