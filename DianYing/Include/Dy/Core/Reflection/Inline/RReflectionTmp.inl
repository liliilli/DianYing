#ifndef GUARD_DY_CORE_REFLECTION_INLINE_RREFLECTIONTMP_INL
#define GUARD_DY_CORE_REFLECTION_INLINE_RREFLECTIONTMP_INL
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

namespace dy::reflect
{

inline EReflectScopeType DType::GetTypeOf(const std::string& iVariable) const
{
  return mMemberVariables.at(iVariable).mType; 
}

inline size_t DType::GetOffsetOf(const std::string& iVariable) const
{
  return mMemberVariables.at(iVariable).mOffset; 
}

inline bool DType::IsAliasExist(const std::string& iAlias) const 
{
  return mAliasContainer.find(iAlias) != mAliasContainer.end();
}

inline const std::string& DType::GetVarNameOf(const std::string& iAlias) const
{
  return mAliasContainer.at(iAlias);
}

template <typename TType>
size_t DType::GetAddressOf(const TType& iInstance, const std::string& iVariable) const
{
  return (size_t)(&iInstance) + this->GetOffsetOf(iVariable); 
}

template <typename TValueType, typename TType>
TValueType& DType::GetValueOf(TType& iInstance, const std::string& iVariable) const
{
  if (ToVariableType<TValueType>::Value != this->GetTypeOf(iVariable))
  {
    throw std::runtime_error("Failed to get value");
  }
  return *static_cast<TValueType*>(this->GetAddressOf(iInstance, iVariable));
}

template <typename TValueType, typename TType>
const TValueType& DType::GetValueOf(const TType& iInstance, const std::string& iVariable) const
{
  if (ToVariableType<TValueType>::Value != this->GetTypeOf(iVariable))
  {
    throw std::runtime_error("Failed to get value");
  }
  return *((const TValueType*)(this->GetAddressOf(iInstance, iVariable)));
}

template <typename TType>
RRegisterType<TType>::RRegisterType(const std::string& iTypeName, const size_t iStructSize)
{
  sTypeName = iTypeName;
  GetTypeContainer()[sTypeName] = DType();
  GetTypeContainer()[sTypeName].mTypeSize = iStructSize;
}

} /// ::dy::reflect namespace

#endif /// GUARD_DY_CORE_REFLECTION_INLINE_RREFLECTIONTMP_INL