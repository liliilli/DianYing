#ifndef GUARD_DY_MANAGEMENT_INTENRAL_RENDER_FDyModelHandlerManager_H
#define GUARD_DY_MANAGEMENT_INTENRAL_RENDER_FDyModelHandlerManager_H
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
#include <Dy/Management/Type/Render/DDyModelHandler.h>

namespace dy
{

/// @class FDyModelHandlerManager
/// @brief Internal model handler manager. \n 
/// This manager initialized and released automatically in MDyRendering.
class FDyModelHandlerManager final : public IDySingleton<FDyModelHandlerManager>
{
public:
  MDY_SINGLETON_DERIVED(FDyModelHandlerManager);
  MDY_SINGLETON_PROPERTIES(FDyModelHandlerManager);

private:

  /// @brief 
  std::unordered_map<std::string, DDyModelHandler> mModelHandlerContainer;
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_INTENRAL_RENDER_FDyModelHandlerManager_H