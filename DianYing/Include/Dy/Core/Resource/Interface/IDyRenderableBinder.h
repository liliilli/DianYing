#ifndef GUARD_DY_CORE_RESOURCE_INTERFACE_IDyRenderableBinder_H
#define GUARD_DY_CORE_RESOURCE_INTERFACE_IDyRenderableBinder_H
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

#include <Dy/Meta/Type/EResourceType.h>

namespace dy
{

/// @struct IDyRenderableBinder
/// @brief  Renderable binder.
struct IDyRenderableBinder
{
public:
  IDyRenderableBinder(EResourceType type) : mRenderType{type} {};
  virtual ~IDyRenderableBinder() = 0;

  EResourceType mRenderType = EResourceType::NoneError;
};

inline IDyRenderableBinder::~IDyRenderableBinder() = default;

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_INTERFACE_IDyRenderableBinder_H