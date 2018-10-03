#ifndef GUARD_DY_COMPONENT_INTERNAL_CDYEMPTYTRANSFORM_H
#define GUARD_DY_COMPONENT_INTERNAL_CDYEMPTYTRANSFORM_H
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

#include <Dy/Component/Abstract/ADyBaseTransform.h>

namespace dy
{

///
/// @class CDyEmptyTransform
/// @brief
/// @TODO SCRIPT THIS
///
class CDyEmptyTransform final : public ADyBaseTransform
{
public:
  CDyEmptyTransform(FDyActor& actorReference);
  virtual ~CDyEmptyTransform() = default;

  CDyEmptyTransform(const CDyEmptyTransform&)                                 = delete;
  CDyEmptyTransform& operator=(const CDyEmptyTransform&)                      = delete;
  CDyEmptyTransform(CDyEmptyTransform&& instance)                   noexcept  = default;
  CDyEmptyTransform& operator=(CDyEmptyTransform&& instance)        noexcept  = default;



  MDY_SET_TYPEMATCH_FUNCTION(::dy::ADyBaseTransform, CDyEmptyTransform);
  MDY_SET_CRC32_HASH_WITH_TYPE(CDyEmptyTransform);
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_INTERNAL_CDYEMPTYTRANSFORM_H