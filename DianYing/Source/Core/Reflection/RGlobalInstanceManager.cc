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
#include <Dy/Core/Reflection/RGlobalInstanceManager.h>
#include <Dy/Helper/Library/HelperContainer.h>

namespace dy::reflect
{

bool RGlobalInstanceManager::IsGlobalInstanceExist(const char* iSpecifier)
{
  const auto hashVal = ::dy::hash::DyToCrc32Hash(iSpecifier);

  auto& map = GetInstanceMap();
  return Contains(map, hashVal);
}

bool RGlobalInstanceManager::IsGlobalInstanceExist(const std::string& iSpecifier)
{
  return IsGlobalInstanceExist(iSpecifier.c_str());
}

ADyGlobalInstance* RGlobalInstanceManager::GetGlobalInstance(const char* iSpecifier) noexcept
{
  const auto hashVal = ::dy::hash::DyToCrc32Hash(iSpecifier);
  
  auto& map = GetInstanceMap();
  if (Contains(map, hashVal) == false) { return nullptr; }

  return map.at(hashVal).get();
}

ADyGlobalInstance* RGlobalInstanceManager::GetGlobalInstance(const std::string& iSpecifier) noexcept
{
  return GetGlobalInstance(iSpecifier.c_str());
}

} /// ::dy::reflect namespace