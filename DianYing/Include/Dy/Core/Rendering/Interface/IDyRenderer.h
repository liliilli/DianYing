#ifndef GUARD_DY_CORE_RENDERING_INTERFACE_IDYRENDERER_H
#define GUARD_DY_CORE_RENDERING_INTERFACE_IDYRENDERER_H
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

class IDyRenderer
{
public:
  /// @brief Check rendering phase is ready.
  [[nodiscard]] virtual bool IsReady() const noexcept = 0;
  /// @brief Try setup rendering, if failed, return DY_FAILURE.
  [[nodiscard]] virtual EDySuccess TrySetupRendering() = 0;
  /// @brief Rendering deferred contexts to default framebuffer.
  virtual void RenderScreen() = 0;
  /// @brief Clear properties of given framebuffer.
  virtual void Clear() = 0;

protected:
  ~IDyRenderer() = default;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RENDERING_INTERFACE_IDYRENDERER_H