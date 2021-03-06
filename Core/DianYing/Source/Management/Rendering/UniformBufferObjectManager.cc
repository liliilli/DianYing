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
#include <Dy/Management/Rendering/MUniformBufferObject.h>
#include <Dy/Management/MLog.h>
#include <Dy/Helper/Library/HelperContainer.h>
#include <Dy/Include/GlInclude.h>
#include <Dy/Helper/System/Assertion.h>

//!
//! Forward declaration & Local translation unit data.
//!

namespace dy
{

MDY_SET_IMMUTABLE_STRING(sFunc_CreateUboContainer, "MUniformBufferObject::CreateUboContainer");

} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

EDySuccess MUniformBufferObject::pfInitialize()
{ // Do nothing.
  return EDySuccess::DY_SUCCESS;
}

EDySuccess MUniformBufferObject::pfRelease()
{ 
  // Release all UBO buffer before release management instance.
  for (auto it = this->mUboMap.begin(); it != this->mUboMap.end();)
  {
    const DDyUboInstanceInformation& instance = *it->second;
    const TU32 bufferIndex  = instance.GetBufferInternalIndex();
    const TU32 bindingIndex = instance.mBindingIndex;
    glDeleteBuffers(1, &bufferIndex);

    it = this->mUboMap.erase(it);
    this->mBeingUsedUboBufferIndexSet.erase(bindingIndex);
  }

  // Make uniform buffer object buffer space
  return EDySuccess::DY_SUCCESS;
}

EDySuccess MUniformBufferObject::CreateUboContainer(_MIN_ const PDyUboConstructionDescriptor& descriptor)
{ 
  // Duplication check
  if (this->GetUboContainer(descriptor.mUboSpecifierName) != nullptr)
  {
    DyPushLogError("{} | Failed to create UBO container. Name is duplicated. | Name : {}",
                  (sFunc_CreateUboContainer),
                  descriptor.mUboSpecifierName);
    return EDySuccess::DY_FAILURE;
  }

  // Binding index duplication check
  if (Contains(this->mBeingUsedUboBufferIndexSet, descriptor.mBindingIndex) == true)
  {
    DyPushLogError("{} | Failed to create UBO container. Binding Index is duplicated. | Binding Index : {}",
                  (sFunc_CreateUboContainer),
                  descriptor.mBindingIndex);
    return EDySuccess::DY_FAILURE;
  }

  // Check array size and element byte size is not specified correctly.
  if (descriptor.mUboArraySize <= 0)    { return EDySuccess::DY_FAILURE; }
  if (descriptor.mUboElementSize <= 0)  { return EDySuccess::DY_FAILURE; }

  // GL INTERNAL SPECIFICATION COMPARISON INTEGRITY TEST

  // ~~~ AFTER THAT.... CREATE AND ISSUE BINDING ID.
  auto [it, result] = this->mUboMap.try_emplace(descriptor.mUboSpecifierName, nullptr);
  MDY_ASSERT_MSG(result == true, "Unexpected error occurred.");

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
  return EDySuccess::DY_SUCCESS;
}

const DDyUboInstanceInformation* MUniformBufferObject::GetUboContainer(_MIN_ const std::string& specifier)
{
  if (auto it = this->mUboMap.find(specifier); it != this->mUboMap.end()) { return it->second.get(); }
  else                                                                    { return nullptr; }
}

EDySuccess MUniformBufferObject::UpdateUboContainer(
  const std::string& specifier,
  TU32 bufferStartByte,
  TU32 bufferWrapSize,
  const void* bufferCopyPtr)
{ 
  // Validation test :: unvalid specifier test
  const DDyUboInstanceInformation* uboPtr = this->GetUboContainer(specifier);
  if (uboPtr == nullptr)
  {
    DyPushLogError(
      "{} | Failed to update UBO container. Given name is not exist. | Name : {}",
      (sFunc_CreateUboContainer), specifier);
    return EDySuccess::DY_FAILURE;
  }

  // Validation Test :: OOB Test
  if (bufferStartByte + bufferWrapSize > uboPtr->GetContainerSize())
  {
    DyPushLogError(
      "{} | Failed to update UBO container. Buffer size out of bound detected. | {} + {} > {}",
      (sFunc_CreateUboContainer), bufferStartByte, bufferWrapSize, uboPtr->GetContainerSize());
    return EDySuccess::DY_FAILURE;
  }

  // Copy buffer data with [bufferStartByte, bufferWrapSize + bufferStartByte).
  glBindBuffer    (GL_UNIFORM_BUFFER, uboPtr->GetBufferInternalIndex());
  glBufferSubData (GL_UNIFORM_BUFFER, bufferStartByte, bufferWrapSize, bufferCopyPtr);
  glBindBuffer    (GL_UNIFORM_BUFFER, 0);
  return EDySuccess::DY_SUCCESS;
}

EDySuccess MUniformBufferObject::ClearUboContainer(
  const std::string& specifier,
  TU32 bufferStartByte,
  TU32 bufferWrapSize)
{ 
  // Validation test :: unvalid specifier test
  const DDyUboInstanceInformation* uboPtr = this->GetUboContainer(specifier);
  if (MDY_CHECK_ISNULL(uboPtr))
  {
    DyPushLogError(
      "{} | Failed to clear UBO container. Given name is not exist. | Name : {}", 
      (sFunc_CreateUboContainer), specifier);
    return EDySuccess::DY_FAILURE;
  }

  // Validation Test :: OOB Test
  if (bufferStartByte + bufferWrapSize > uboPtr->GetContainerSize())
  {
    DyPushLogError(
      "{} | Failed to clear UBO container. Buffer size out of bound detected. | {} + {} > {}",
      (sFunc_CreateUboContainer), bufferStartByte, bufferWrapSize, uboPtr->GetContainerSize());
    return EDySuccess::DY_FAILURE;
  }

  // Clear buffer with 0.
  static constexpr float nullValue = 0.f;
  glBindBuffer(GL_UNIFORM_BUFFER, uboPtr->GetBufferInternalIndex());
  glClearBufferSubData(GL_UNIFORM_BUFFER, GL_RGBA, bufferStartByte, bufferWrapSize, GL_RED, GL_FLOAT, &nullValue);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
  return EDySuccess::DY_SUCCESS;
}

EDySuccess MUniformBufferObject::RemoveUboContainer(_MIN_ const std::string& specifier)
{ 
  // Validation test :: unvalid specifier test
  const DDyUboInstanceInformation* uboPtr = this->GetUboContainer(specifier);
  if (MDY_CHECK_ISNULL(uboPtr))
  {
    DyPushLogError("{} | Failed to clear UBO container. Given name is not exist. | Name : {}",
        (sFunc_CreateUboContainer),
        specifier);
    return EDySuccess::DY_FAILURE;
  }

  // Delete logic
  glDeleteBuffers(1, &uboPtr->mBufferIndex);
  this->mBeingUsedUboBufferIndexSet.erase(uboPtr->mBindingIndex);
  this->mUboMap.erase(specifier);
  return EDySuccess::DY_SUCCESS;
}

} /// ::dy namespace