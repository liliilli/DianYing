#ifndef GUARD_DY_HELPER_STRUCTURE_H
#define GUARD_DY_HELPER_STRUCTURE_H

#include <cstdint>
#include <vector>

/// @struct DHeaderStructure
struct DHeaderStructure final
{
  struct DBufferLength final
  {
    uint64_t mPlainBufferLength      = 0;
    uint64_t mCompressedBufferLength = 0;
  };
  using TBufferLenList = std::vector<DBufferLength>;

  uint64_t mPtrPreviousFontAddress        = 0;
  uint64_t mPtrNextFontAddress            = 0;
  DBufferLength mInformationBufferLength  = {};
  TBufferLenList mImageBufferLength       = {};
};

struct DBufferSerializer final
{
  std::vector<char> mBuffer;

  DBufferSerializer& operator<<(const char& buffer)
  {
    this->mBuffer.emplace_back(buffer);
    return *this;
  }

  DBufferSerializer& operator<<(const uint64_t& buffer)
  {
    constexpr auto  loopCount = sizeof(buffer) / sizeof(unsigned char);
    const auto*     ptr       = reinterpret_cast<const char*>(&buffer);
    for (auto i = 0u; i < loopCount; ++i)
    {
      *this << *ptr;
      ptr++;
    }
    return *this;
  }

  DBufferSerializer& operator<<(const DHeaderStructure::DBufferLength& buffer)
  {
    *this << buffer.mPlainBufferLength << buffer.mCompressedBufferLength;
    return *this;
  }

  DBufferSerializer& operator<<(const std::vector<char>& buffer)
  {
    for (const auto& chr : buffer) { *this << chr; }
    return *this;
  }

  template <class TType, class TAllocator = std::allocator<TType>>
  DBufferSerializer& operator<<(const std::vector<TType, TAllocator>& buffer)
  {
    for (const auto& item : buffer) { *this << item; };
    return *this;
  }
};

#endif /// GUARD_DY_HELPER_STRUCTURE_H
