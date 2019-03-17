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
#include <Dy/Helper/Library/HelperString.h>
#include <random>

namespace dy
{

MDY_NODISCARD std::string CreateRandomString(TU32 length)
{
  static std::default_random_engine Rng(std::random_device{}());
  auto randchar = []() -> char
  {
    constexpr char charset[]  = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    constexpr auto maxIndex   = (sizeof(charset) - 1);
    return charset[Rng() % maxIndex];
  };

  std::string str(length, 0);
  std::generate_n(str.begin(), length, randchar);
  return str;
}

bool HasPostfix(const std::string& iString, const std::string& iPostfix)
{
  if (const auto id = iString.find(iPostfix.c_str()); id == std::string::npos)
  {
    return false;
  }
  else if (iString.size() - id != iPostfix.size())
  {
    return false;
  }

  return true;
}

std::string TryRemovePostfix(const std::string& iString, const std::string& iPostfix)
{
  if (HasPostfix(iString, iPostfix) == false)
  {
    return iString;
  }

  return iString.substr(0, iString.find(iPostfix.c_str()));
}

  void ToLower(std::string& ioString)
{
  std::transform(
    MDY_BIND_BEGIN_END(ioString), ioString.begin(), 
    [](const char in) 
    {
      if (in <= 'Z' && in >= 'A') { return static_cast<char>(in - ('Z' - 'z')); };
      return in;
    }
  );
}

std::string ToLowerCopy(const std::string& iString)
{
  std::string resultString(iString);
  std::transform(
    MDY_BIND_BEGIN_END(resultString), resultString.begin(), 
    [](const char in) 
    {
      if (in <= 'Z' && in >= 'A') { return static_cast<char>(in - ('Z' - 'z')); };
      return in;
    }
  );

  return resultString;
}

void ToUpper(std::string& ioString)
{
  std::transform(
    MDY_BIND_BEGIN_END(ioString), ioString.begin(), 
    [](const char in) 
    {
      if (in <= 'z' && in >= 'a') { return static_cast<char>(in + ('Z' - 'z')); };
      return in;
    }
  );
}

std::string ToUpperCopy(const std::string& iString)
{
  std::string resultString(iString);
  std::transform(
    MDY_BIND_BEGIN_END(resultString), resultString.begin(), 
    [](const char in) 
    {
      if (in <= 'z' && in >= 'a') { return static_cast<char>(in + ('Z' - 'z')); };
      return in;
    }
  );

  return resultString;
}

} /// ::dy namespace