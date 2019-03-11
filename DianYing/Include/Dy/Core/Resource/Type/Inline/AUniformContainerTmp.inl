#ifndef GUARD_DY_CORE_TYPE_SHADER_ADYUNIFORMCONTAINER_TMP_H
#define GUARD_DY_CORE_TYPE_SHADER_ADYUNIFORMCONTAINER_TMP_H
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

namespace dy
{

template <EDyUniformVariableType TType>
EDySuccess ADyUniformContainer::TryUpdateUniform(
  const std::string& iSpecifier, 
  const typename MDY_PRIVATE(UniformBinder)<TType>::ValueType& iValue)
{
  const auto it = this->mUniformMap.find(iSpecifier);
  if (it == this->mUniformMap.end())
  { // If not found, just insert it anyway.
    auto [createdPair, _] = this->mUniformMap.try_emplace(
      iSpecifier, 
      std::make_unique<FDyUniformValue<TType>>(-1, iValue)
    );
    DyPushLogError("Could not find uniform value but insert anyway as id -1. {}", iSpecifier);
    return DY_FAILURE;
  }
  else
  { // Check type but not matched, pass it.
    auto& [_, smtptrInstance] = *it;
    if (smtptrInstance->mType != TType)
    {
      DyPushLogError("Could not insert uniform value becasue of different type. {}", iSpecifier);
      return DY_FAILURE;
    }
    // In case of success.
    auto* ptrInstance = static_cast<FDyUniformValue<TType>*>(smtptrInstance.get());
    if (ptrInstance->mValue == iValue) { return DY_SUCCESS; }

    ptrInstance->mValue = iValue;
    this->mUpdatedItemList.emplace_back(ptrInstance);
    return DY_SUCCESS;
  }
}

} /// ::dy namespace

#endif /// GUARD_DY_CORE_TYPE_SHADER_ADYUNIFORMCONTAINER_TMP_H