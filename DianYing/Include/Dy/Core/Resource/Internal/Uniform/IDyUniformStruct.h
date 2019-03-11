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
#include <Dy/Core/Resource/Internal/Uniform/IDyUniformValueBase.h>

namespace dy
{

/// @struct IDyUniformStructBase
/// @brief base uniform struct type.
struct IDyUniformStructBase
{
public:
  IDyUniformStructBase(const std::string& iPrefix) : mPrefix{iPrefix} {}; 
  virtual ~IDyUniformStructBase() = 0;

  std::string mPrefix;
};

inline IDyUniformStructBase::~IDyUniformStructBase() = default;

/// @struct IDyUniformStructItem
/// @brief Uniform struct value base type
struct IDyUniformStructItem final : public IDyUniformStructBase
{
public:
  std::vector<std::unique_ptr<IDyUniformValueBase>> mMemberValues;

  IDyUniformStructItem(const std::string& iPrefix) : IDyUniformStructBase{iPrefix} {};
};

/// @struct IDyUniformStructList
/// @brief Uniform struct list value base type
struct IDyUniformStructList final : public IDyUniformStructBase
{
public:
  struct DItem final
  {
    std::vector<std::unique_ptr<IDyUniformValueBase>> mMemberValues;
  };
  
  std::string         mPrefix;
  std::vector<DItem>  mItems;

  IDyUniformStructList(const std::string& iPrefix) 
    : IDyUniformStructBase{iPrefix} {};
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_INTENRAL_UNIFORM_IDyUniformStruct_H