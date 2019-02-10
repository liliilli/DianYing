#ifndef GUARD_DY_MANAGEMENT_INTERNAL_MDyDebug_H
#define GUARD_DY_MANAGEMENT_INTERNAL_MDyDebug_H
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

#include <Dy/Management/Interface/ISingletonCrtp.h>

namespace dy
{
  
/// @class MDyDebug
/// @brief Debug manager. Manages setting & rendering of debug ui & input etc.
/// This must be initiated only in debug mode `-d`.
///
/// @WARNING This management does not initiate imgui directly.
/// imgui is initiated in rendering manager, so user can use imgui in shipping mode.
class MDyDebug final : public IDySingleton<MDyDebug>
{
public:
  MDY_SINGLETON_DERIVED(MDyDebug);
  MDY_SINGLETON_PROPERTIES(MDyDebug);


};


} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_INTERNAL_MDyDebug_H