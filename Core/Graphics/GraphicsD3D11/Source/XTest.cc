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

/// @brief This function is not implemented yet.
static auto InitializeDirectX11 = []()
{
  #ifdef false
  MDY_CALL_ASSERT_SUCCESS(DyWin32InitializeWindow(hInstance));
  MDY_CALL_ASSERT_SUCCESS(DyD11InitializeDirect3D());

  ShowWindow(windowHandle, SW_SHOW);
  SetForegroundWindow(windowHandle);
  SetFocus(windowHandle);

  DyD11CreateEffectFx();
  DyD11BindVertexLayout();
  DyD11CreateVertexBuffer();
  DyD11RenderLoop();
  return EDySuccess::DY_FAILURE;
  #endif
};

} /// ::dy namespace