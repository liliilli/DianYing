#ifndef GUARD_DY_CORE_RESOURCE_INTENRAL_UNIFORM_IDyUniformStruct_H
#define GUARD_DY_CORE_RESOURCE_INTENRAL_UNIFORM_IDyUniformStruct_H
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

#include <string>
#include <Dy/Core/Resource/Internal/Uniform/IUniformValueBase.h>

namespace dy
{

/// @struct IUniformStructBase
/// @brief base uniform struct type.
struct IUniformStructBase
{
public:
  IUniformStructBase(const std::string& iPrefix) : mPrefix{iPrefix} {}; 
  virtual ~IUniformStructBase() = 0;

  std::string mPrefix;
};

inline IUniformStructBase::~IUniformStructBase() = default;

/// @struct IUniformStructItem
/// @brief Uniform struct value base type
struct IUniformStructItem final : public IUniformStructBase
{
public:
  std::vector<std::unique_ptr<IUniformValueBase>> mMemberValues;

  IUniformStructItem(const std::string& iPrefix) : IUniformStructBase{iPrefix} {};
};

/// @struct IUniformStructList
/// @brief Uniform struct list value base type
struct IUniformStructList final : public IUniformStructBase
{
public:
  struct DItem final
  {
    std::vector<std::unique_ptr<IUniformValueBase>> mMemberValues;
  };
  
  std::string         mPrefix;
  std::vector<DItem>  mItems;

  IUniformStructList(const std::string& iPrefix) 
    : IUniformStructBase{iPrefix} {};
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_INTENRAL_UNIFORM_IDyUniformStruct_H