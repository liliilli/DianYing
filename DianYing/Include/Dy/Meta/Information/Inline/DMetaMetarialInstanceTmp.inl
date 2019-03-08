#ifndef GUARD_DY_META_INFORMATION_INLINE_DMETAMETARIALINSTANCETMP_INL
#define GUARD_DY_META_INFORMATION_INLINE_DMETAMETARIALINSTANCETMP_INL
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

#include <Dy/Core/Resource/Type/Uniform/UniformValueTypes.h>

namespace dy
{

template <EDyUniformVariableType TType>
void PDyMaterialInstanceMetaInfo::InsertValue(
  PDyMaterialInstanceMetaInfo& ioMaterialInfo, 
  const std::string& iSpecifier, 
  const typename MDY_PRIVATE(UniformBinder)<TType>::ValueType& iValue)
{
  const auto it = ioMaterialInfo.mUniformValues.find(iSpecifier);
  MDY_ASSERT_MSG_FORCE(
    it == ioMaterialInfo.mUniformValues.end(),
    "Initial uniform value's string must not be duplicated with others."
  );

  // In case of success.
  auto [_, __] = ioMaterialInfo.mUniformValues.try_emplace
  (
    iSpecifier, 
    std::make_unique<FDyUniformValue<TType>>(-1, iValue)
  );
}

} /// ::dy namespace

#endif /// GUARD_DY_META_INFORMATION_INLINE_DMETAMETARIALINSTANCETMP_INL