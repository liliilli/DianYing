#ifndef GUARD_DY_ELEMENT_INTERNAL_TDyIdDistributor_H
#define GUARD_DY_ELEMENT_INTERNAL_TDyIdDistributor_H
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

#include <unordered_set>

namespace dy
{

/// @class TDyIdDistributor
/// @brief Counter Id distributor.
template <typename TIdType, typename TType>
class TDyIdDistributor
{
public:
  TDyIdDistributor() : mIsGivenIdValid{true} { this->GetNewUniqueId(); }
  virtual ~TDyIdDistributor() { this->ResetGivenUniqueId(); }
  
  TDyIdDistributor(const TDyIdDistributor&) = delete; 
  TDyIdDistributor& operator=(const TDyIdDistributor&) = delete;
  
  TDyIdDistributor(TDyIdDistributor&& from) noexcept : 
      mGivenId{from.mGivenId}, mIsGivenIdValid{from.mIsGivenIdValid}
  {
    from.mGivenId         = kExclusiveId;
    from.mIsGivenIdValid  = false;
  }
  TDyIdDistributor& operator=(TDyIdDistributor&& from) noexcept
  {
    if (this == &from) { return *this; }
    this->mGivenId = from.mGivenId;
    this->mIsGivenIdValid = from.mIsGivenIdValid;

    from.mGivenId         = kExclusiveId;
    from.mIsGivenIdValid  = false;
    return *this;
  }

  void ResetGivenUniqueId()
  {
    if (this->mIsGivenIdValid == false) { return; }

    this->mIsGivenIdValid = false;
    sUsingIdList.erase(this->mGivenId);
    this->mGivenId = kExclusiveId;
  }

  void GetNewUniqueId()
  {
    // Try reset given unique id.
    this->ResetGivenUniqueId();

    // Loop to find available id.
    while(sIdCounter == kExclusiveId 
      ||  sUsingIdList.count(sIdCounter) >= 1) 
    { 
      ++sIdCounter; 
    }

    // Set
    this->mGivenId = sIdCounter++;
    sUsingIdList.emplace(this->mGivenId);
  }

  static constexpr TIdType kExclusiveId = TIdType{};

protected:
  /// @brief Given id.
  TIdType mGivenId;
  /// @brief Check id is given and valid.
  bool    mIsGivenIdValid = false;

private:
  inline static std::unordered_set<TIdType> sUsingIdList {};
  inline static TIdType sIdCounter;
};

} /// ::dy namespace

#endif /// GUARD_DY_ELEMENT_INTERNAL_TDyIdDistributor_H