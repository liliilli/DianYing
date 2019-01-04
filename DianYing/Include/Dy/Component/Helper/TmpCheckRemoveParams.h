#ifndef GUARD_DY_COMPONENT_HELPER_TMPCHECKREMOVEPARAMS_H
#define GUARD_DY_COMPONENT_HELPER_TMPCHECKREMOVEPARAMS_H
///
/// MIT License
/// Copyright (c) 2018 Jongmin Yun
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.
///

#include <Dy/Component/CDyTransform.h>
#include <Dy/Component/Actor/CDyActorScript.h>
#include <Dy/Helper/Internal/TmpCheckTypeParams.h>

namespace dy
{

///
/// @brief  Check type parameters which to use RemoveComponent() properly in compile time.
/// This function must be successful when called.
///
/// @tparam TComponentType  Arbitary class type which is based on ADyBaseComponent.
/// @tparam TArgs           Arbitary type parameter to be used in FDyActor::RemoveComponent().
///
template <typename TComponentType, typename... TArgs>
constexpr void DyCheckComponentRemoveFunctionParams() noexcept
{
  static_assert(std::is_base_of_v<ADyBaseComponent, TComponentType>,
                "Failed compile test. TComponentType is not derived type of ADyBaseTransform.");

  if constexpr (std::is_same_v<CDyActorScript, TComponentType>)
  {
    static_assert(MetaTest<const std::string&>::TypeMatched<TArgs...>::template Result<>::value,
                  "Failed compile test. Could not remove CDyScript instance.");
  }
  else if constexpr (std::is_same_v<CDyTransform, TComponentType>)
  {
    static_assert(!std::is_same_v<CDyTransform, TComponentType>,
                  "Can not delete CDyTransform and CDyEmptyTransform from component list.");
  }
  else
  {
    static_assert(sizeof...(TArgs) == 0, "Failed compile test. Parameter count must be zero.");
  }
}

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_HELPER_TMPCHECKREMOVEPARAMS_H