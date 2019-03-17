#include <precompiled.h>

#include <Dy/Helper/Library/HelperIO.h>

namespace dy
{

std::optional<std::vector<char>> 
GetBufferFromFile(const std::string& iFilePath)
{
  // ReSharper disable CppDeprecatedEntity
  std::FILE* fd = std::fopen(iFilePath.c_str(), "r");
  if(fd == nullptr) 
  {
    return std::nullopt;
  }

  // ReSharper restore CppDeprecatedEntity
  std::fseek(fd, 0, SEEK_END);
  const auto size = ftell(fd);
  std::fseek(fd, 0, SEEK_SET);

  std::vector<char> buffer(size + 1);
  std::fread(buffer.data(), sizeof(char), size, fd);
  std::fclose(fd);

  return std::move(buffer);
}

std::optional<std::vector<char>>  
GetBufferFromFile(const std::filesystem::path& iFilePath)
{
  return GetBufferFromFile(iFilePath.string());
}

} /// ::dy namespace
