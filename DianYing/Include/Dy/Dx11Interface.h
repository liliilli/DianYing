#ifndef GUARD_DY_DIRECTX11_INTERFACE_H
#define GUARD_DY_DIRECTX11_INTERFACE_H
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

#include <Dy/Helper/enum_flags.h>

namespace dy
{

//!
//! DirectX11 API
//!

EDySuccess                DyD11InitializeDirect3D();
void                      DyD11RenderLoop();
void                      DyD11RenderFrame();

/// @todo Chapter 6
struct DVertex1
{
  DirectX::XMFLOAT3 position;
  DirectX::XMFLOAT4 color;
};

struct DVectex2
{
  DirectX::XMFLOAT3 position;
  DirectX::XMFLOAT2 normal;
  DirectX::XMFLOAT2 texCoord0;
  DirectX::XMFLOAT2 texCoord1;
};

//!
//! DirectX12 API
//!

void                  DyD12RenderLoop();
void                  DyD12RenderFrame();

ID3D11Device*         d11Device         = nullptr;
ID3D11DeviceContext*  d11DeviceContext  = nullptr;


} /// ::dy namespace

#endif /// GUARD_DY_DIRECTX11_INTERFACE_H