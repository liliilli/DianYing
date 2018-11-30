#ifndef GUARD_DY_BUILTIN_MODEL_BOX_H
#define GUARD_DY_BUILTIN_MODEL_BOX_H
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

#include <string_view>
#include <Dy/Builtin/Abstract/ADyModelResource.h>

#define MDY_REGISTER_RESOURCE_MODEL(__MAType__, __MASpecifierName__) \
  MDY_REGISTER_RESOURCE_WITH_SPECIFIER(__MAType__, __MASpecifierName__) \
  private: \
  static PDyModelConstructionVertexDescriptor& __Get() noexcept \
  { \
    static PDyModelConstructionVertexDescriptor instance{}; \
    return instance; \
  } \
  void ConstructBuffer(_MOUT_ PDyModelConstructionVertexDescriptor& buffer) noexcept; \
  public: \
    __MAType__() \
    { \
      ConstructBuffer(__Get()); \
      this->mPtrBuffer = &__Get(); \
    }

namespace dy::builtin
{

///
/// @class FDyBuiltinModelBox
/// @brief Create model box information and resources.
///
class FDyBuiltinModelBox final : public ADyModelResource
{
  MDY_REGISTER_RESOURCE_MODEL(FDyBuiltinModelBox, "dyBtModelBox");
};

} /// ::dy::builtin namespace

#endif /// GUARD_DY_BUILTIN_MODEL_BOX_H