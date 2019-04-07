#ifndef GUARD_DY_MANAGEMENT_INTERFACE_ISINGLETONCRTP_H
#define GUARD_DY_MANAGEMENT_INTERFACE_ISINGLETONCRTP_H
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

#include <atomic>

#include <Dy/Helper/System/Macroes.h>
#include <Expr/ISingleton.h>

namespace dy
{
  
/// @macro MDY_SINGLETON_PROPERTIES
/// This macro must not be attached to whichever class does not inherits ISingleton<>.
#define MDY_SINGLETON_PROPERTIES(__MASingletonType__) EXPR_SINGLETON_PROPERTIES(__MASingletonType__)

/// @macro MDY_SINGLETON_DERIVED
/// This macro must not be attached to whichever class does not inherits ISingleton<>.
#define MDY_SINGLETON_DERIVED(__MADerivedSingletonType__) EXPR_SINGLETON_DERIVED(__MADerivedSingletonType__)

/// @def DY_PIMPL_SINGELTON_DERIVED
/// This macro must not be attached to whichever class does not inherit ISingleton<>.
#define DY_PIMPL_SINGELTON_DERIVED(__MADerivedSingletonType__) \
    DY_DECLARE_PIMPL();                                   \
  public:                                                 \
    __MADerivedSingletonType__() = default;               \
    [[nodiscard]] EDySuccess pfInitialize();              \
    [[nodiscard]] EDySuccess pfRelease();                 \
    friend class ISingleton<__MADerivedSingletonType__>;  \
public:                                                   \
    virtual ~__MADerivedSingletonType__(); 

using expr::ISingleton;

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_INTERFACE_ISINGLETONCRTP_H