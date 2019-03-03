#ifndef GUARD_DY_CORE_TYPE_SHADER_ADYUNIFORMCONTAINER_H
#define GUARD_DY_CORE_TYPE_SHADER_ADYUNIFORMCONTAINER_H
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

#include <Dy/Helper/ContainerHelper.h>
#include <Dy/Helper/System/Idioms.h>
#include <Dy/Core/Resource/Internal/ShaderType.h>
#include <Dy/Core/Resource/Internal/Uniform/IDyUniformValueBase.h>
#include <Dy/Core/Resource/Type/Shader/TemplateUniformType.h>
#include <Dy/Core/Resource/Type/Uniform/UniformValueTypes.h>

//!
//! Forward declaration
//!

namespace dy
{
class FDyShaderResource;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

class ADyUniformContainer
{
public:
  virtual ~ADyUniformContainer() = 0;

  /// @brief Try clear uniform list.
  void __TryClearUniformList();
  /// @brief Try construct default uniform variable list.
  /// If already have value, do nothing.
  void __TryConstructDefaultUniformList(_MIN_ const FDyShaderResource& iResc);

  template <EDyUniformVariableType TType>
  EDySuccess TryUpdateUniform(
      _MIN_ const std::string& iSpecifier, 
      _MIN_ const typename MDY_PRIVATE(UniformBinder)<TType>::ValueType& iValue)
  {
    const auto it = this->mUniformMap.find(iSpecifier);
    if (it == this->mUniformMap.end())
    { // If not found, just insert it anyway.
      auto [createdPair, _] = this->mUniformMap.try_emplace(iSpecifier, std::make_unique<FDyUniformValue<TType>>(-1, iValue)); \
      DyPushLogError("Could not find uniform value but insert anyway as id -1. {}", iSpecifier);
      return DY_FAILURE;
    }
    else
    { // Check type but not matched, pass it.
      auto& [_, smtptrInstance] = *it;
      if (smtptrInstance->mType != TType)
      {
        DyPushLogError("Could not insert uniform value becasue of different type. {}", iSpecifier);
        return DY_FAILURE;
      }
      // In case of success.
      auto* ptrInstance = static_cast<FDyUniformValue<TType>*>(smtptrInstance.get());
      if (ptrInstance->mValue == iValue) { return DY_SUCCESS; }

      ptrInstance->mValue = iValue;
      this->mUpdatedItemList.emplace_back(ptrInstance);
      return DY_SUCCESS;
    }
  }

  /// @brief Try insert texture requisition. \n
  /// If required `insertId` is out of bound of count of available texture, just do nothing but return DY_FAILURE.
  /// When successful, find proper texture type from container, make requisition issue item and insert to queue. \n
  /// This function does not update texture binding status but need to call `TryUpdateUniformList` function.
  EDySuccess TryInsertTextureRequisition(_MIN_ TU32 insertId, _MIN_ TU32 textureId);

  /// @brief Try update uniform variables and new texture requisition. \n
  /// Do nothing when update list is empty or binding flag is not set up
  /// by calling MDY_PRIVATE(TryConstructDefaultUniformList)().
  EDySuccess TryUpdateUniformList();

  /// @brief Get available texture count.
  MDY_NODISCARD TU32 GetAvailableTextureCount() const noexcept;

private:
  /// @struct DTextureUpdateItem
  /// @brief Texture binding requisition item type. This type is used only in ADyUniformContainer.
  struct DTextureUpdateItem final
  {
    TU32 mInsertId  = 0;
    EDyUniformVariableType mType = EDyUniformVariableType::NoneError;
    TU32 mTextureId = 0;

    DTextureUpdateItem(_MIN_ TU32 insertId, _MIN_ EDyUniformVariableType type, _MIN_ TU32 textureId) :
        mInsertId{insertId}, mType{type}, mTextureId{textureId} {};
  };

  template <EDyUniformVariableType TType> using TValueType = typename MDY_PRIVATE(UniformBinder)<TType>::ValueType;

  using TUniformMap         = std::unordered_map<std::string, std::unique_ptr<IDyUniformValueBase>>;
  using TUpdatedItemList    = std::vector<IDyUniformValueBase*>;
  using TUpdatedTextureList = std::vector<DTextureUpdateItem>;

  TUniformMap         mUniformMap {};
  TUpdatedItemList    mUpdatedItemList = {};
  TUpdatedTextureList mUpdatedTextureList = {};

  bool              mIsShaderSetuped = false;
  TU32              mAvailableTextureCount = 0;
};

inline ADyUniformContainer::~ADyUniformContainer() = default;

} /// ::dy namespace

#endif /// !GUARD_DY_CORE_TYPE_SHADER_ADYUNIFORMCONTAINER_H
