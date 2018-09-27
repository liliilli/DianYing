#ifndef GUARD_DY_ELEMENT_OBJECT_H
#define GUARD_DY_ELEMENT_OBJECT_H
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

#include <Dy/Element/Interface/IDyToString.h>

namespace dy
{

///
/// @class FDyObject
/// @brief all base type of all derived element types.
///
class FDyObject : public IDyToString
{
public:
  virtual ~FDyObject() = 0;

  std::string ToString() override = 0;
};

} /// ::dy namespace

#endif /// GUARD_DY_ELEMENT_OBJECT_H