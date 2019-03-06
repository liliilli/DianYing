#include <precompiled.h>
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

#include <Dy/Helper/Type/DUuid.h>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <Dy/Management/LoggingManager.h>

namespace dy
{

DUuid::DUuid(bool iCreateValue) : mUuid()
{
  if (iCreateValue == true)
  {
    // Make new value.
    this->mUuid = boost::uuids::random_generator()();
  }
}

DUuid::DUuid(const std::string& iUuidString, bool iIsStrict) : mUuid()
{
  try
  {
    const boost::uuids::string_generator gen;
    this->mUuid = gen(iUuidString);
  }
  catch (std::runtime_error& err)
  {
    MDY_ASSERT_MSG_FORCE(
      iIsStrict == false, 
      MakeStringU8(
        "Failed to convert given string {} to appropriate uuid. {}", iUuidString, err.what()).c_str());

    DyPushLogError("Failed to convert given string {} to appropriate uuid.", iUuidString); 
  }
}

bool DUuid::HasValue() const
{
  return this->mUuid.is_nil() == false;
}

std::string DUuid::ToString() const noexcept
{
  return boost::lexical_cast<std::string>(this->mUuid);
}

void to_json(nlohmann::json& oJson, const DUuid& iUuid)
{
  oJson = iUuid.ToString();
}

void from_json(const nlohmann::json& iJson, DUuid& oUuid)
{
  const auto resultUuid = DUuid(iJson.get<std::string>());
  std::memcpy(&oUuid.mUuid, &resultUuid.mUuid, oUuid.Size());
}

} /// ::dy namespace
