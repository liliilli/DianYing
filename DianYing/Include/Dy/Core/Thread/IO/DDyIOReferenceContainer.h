#ifndef GUARD_DY_CORE_THREAD_IO_DDYREFERENCECONTAINER_H
#define GUARD_DY_CORE_THREAD_IO_DDYREFERENCECONTAINER_H
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

#include <Dy/Core/Thread/IO/DDyIOReferenceInstance.h>

namespace dy
{

///
/// @class DDyIOReferenceContainer
/// @brief IO Reference container for managing reference counting.
///
class DDyIOReferenceContainer final
{
public:
  ///
  /// @brief Check whether resource's Reference Instance is exist on now as any kind of information.
  /// @param specifier Resource specifier name.
  /// @param type  Resource type.
  ///
  MDY_NODISCARD bool IsReferenceInstanceExist(_MIN_ const std::string& specifier, _MIN_ EDyResourceType type);

  ///
  /// @brief Try update scope of given style's specifier RI of resource type. \n
  /// This function is not checking RI(Reference Instance) is exist or not, so when RI is not exist
  /// may will cause UB (Undefined Behaviour).
  /// \n\n
  /// If found RI's scope is \n
  /// Global < Level \n
  /// Level  < Temporal \n
  /// RI's scope will be changed to big range.
  ///
  MDY_NODISCARD bool TryEnlargeResourceScope(_MIN_ EDyScope scope, _MIN_ const std::string& specifier, _MIN_ EDyResourceType type);

  ///
  /// @brief
  ///
  MDY_NODISCARD EDySuccess CreateReferenceInstance(
      _MIN_ const std::string& specifier,
      _MIN_ EDyResourceType type, _MIN_ EDyResourceStyle style, _MIN_ EDyScope scope);

private:
  TStringHashMap<DDyIOReferenceInstance> mMapTextureReference   = {};
  TStringHashMap<DDyIOReferenceInstance> mMapGLShaderReference  = {};
  TStringHashMap<DDyIOReferenceInstance> mMapModelReference     = {};
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_THREAD_IO_DDYREFERENCECONTAINER_H