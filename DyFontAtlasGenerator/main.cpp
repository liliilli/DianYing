#include "precompiled.h"
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

#include <QtWidgets/QApplication>
#include <Include/DyFontAtlasGenerator.h>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  dy::DyFontAtlasGenerator w;
  w.show();

#ifdef false
  CPaintSurface paintSurface;
  paintSurface.resize(1024, 1024);
  paintSurface.render();
  QImage image = paintSurface.grabFramebuffer();
  image.save(QString("image.png"));
#endif

  return a.exec();
}
