#ifndef GUARD_DY_MANAGEMENT_FONTMANAGER_H
#define GUARD_DY_MANAGEMENT_FONTMANAGER_H
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

#include <Dy/Management/Interface/ISingletonCrtp.h>

namespace dy
{

class MDyFont final : public ISingleton<MDyFont>
{
  MDY_SINGLETON_PROPERTIES(MDyFont);
  MDY_SINGLETON_DERIVED(MDyFont);
public:


private:


};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_FONTMANAGER_H