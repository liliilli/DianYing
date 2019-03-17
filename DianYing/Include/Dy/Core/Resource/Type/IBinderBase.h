#ifndef GUARD_DY_CORE_RESOURCE_TYPE_FDYBINDERBASE_H
#define GUARD_DY_CORE_RESOURCE_TYPE_FDYBINDERBASE_H
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

/// @interface IBinderBase
/// @brief Using interface for binding resource & information instance. Must use this, not __TBaseResourceBinder.
MDY_INTERFACE IBinderBase
{
  virtual ~IBinderBase() = default;

  MDY_NODISCARD virtual bool IsResourceExist() const noexcept = 0;
  virtual void TryUpdateResourcePtr(_MIN_ const void* ptr) noexcept = 0;
  virtual void TryDetachResourcePtr() noexcept = 0;
  virtual void Process() noexcept {};
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_TYPE_FDYBINDERBASE_H