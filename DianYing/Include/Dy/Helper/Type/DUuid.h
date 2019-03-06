#ifndef GUARD_DY_HELPER_TYPE_UUID_H
#define GUARD_DY_HELPER_TYPE_UUID_H
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

#include <cstdint>
#include <string>
#define BOOST_UUID_NO_TYPE_TRAITS 
#include <boost/uuid/uuid.hpp>
#include <nlohmann/json_fwd.hpp>

namespace dy
{

struct DUuid;
void to_json(nlohmann::json& oJson, const DUuid& iMeta);
void from_json(const nlohmann::json& iJson, DUuid& oMeta);

/// @struct DUuid
/// @brief Uuid structure.
struct DUuid final
{
public:
  /// @brief If true, create new value when creating uuid instance.
  /// Otherwise, just let it empty value (00000000-0000-0000-0000-000000000000)
  explicit DUuid(bool iCreateValue);

  /// @brief Check string whether it is uuid string in runtime,
  /// and if true, convert string into uuid value. 
  /// Otherwise, just let it empty with error message.
  ///
  /// If iIsStrict is true, just error and abort program when conversion failed
  /// instead of printing error log.
  explicit DUuid(const std::string& iUuidString, bool iIsStrict = false);

  typedef uint8_t value_type;
  typedef uint8_t& reference;
  typedef uint8_t const& const_reference;
  typedef uint8_t* iterator;
  typedef uint8_t const* const_iterator;
  typedef std::ptrdiff_t difference_type;

  /// @brief Get size of uuid. Returned value is always 16.
  constexpr std::size_t Size() const noexcept 
  { 
    return 16; 
  }

  /// @brief Check uuid has valid value.
  [[nodiscard]] bool HasValue() const;

  /// @brief Return value as string.
  [[nodiscard]] std::string ToString() const noexcept;

  // iteration
  iterator begin() noexcept;
  iterator end() noexcept;
  const_iterator begin() const noexcept;
  const_iterator end() const noexcept;

private:
  boost::uuids::uuid mUuid;

  friend void to_json(nlohmann::json& oJson, const DUuid& iUuid);
  friend void from_json(const nlohmann::json& iJson, DUuid& oUuid);
};

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_TYPE_UUID_H