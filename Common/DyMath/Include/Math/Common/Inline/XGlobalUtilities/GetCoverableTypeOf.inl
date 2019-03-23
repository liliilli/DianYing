#pragma once
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

#include <Math/Common/TGlobalTypes.h>

namespace dy::math 
{

template <EValueCategory ECategory, std::size_t VBitSize> 
struct GetCoverableTypeOf_T final
{
private:
  template <std::size_t BitSize, typename TType> 
  struct TExpression final
  {
    static constexpr bool kResult = (TType::kTypeSize * 8) >= VBitSize;
  };
  template <typename TType>
  struct Unary
  {
    static constexpr bool kResult = TExpression<VBitSize, TType>::kResult;
  };

  using TCategoryList = typename TTypeListOf_T<ECategory>::Type;

public:
  using Type = expr::STypeList::GetTypeIf<TCategoryList, Unary>;
};

} /// ::dy::math namespace