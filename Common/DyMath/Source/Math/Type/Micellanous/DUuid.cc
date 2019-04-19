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

#include <Math/Type/Micellanous/DUuid.h>

#define BOOST_UUID_NO_TYPE_TRAITS 
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <Math/Common/XGlobalMacroes.h>

namespace dy::math
{

DUuid::DUuid(bool iCreateValue)
{
  if (iCreateValue == true)
  {
    // Make new value.
     this->mUuid = new (std::nothrow) boost::uuids::uuid();
    *this->mUuid = boost::uuids::random_generator()();
  }
}

DUuid::DUuid(const std::string& iUuidString, [[maybe_unused]] bool iIsStrict) 
{
  const boost::uuids::string_generator gen;

   this->mUuid = new (std::nothrow) boost::uuids::uuid();
  *this->mUuid = gen(iUuidString);
}

DUuid::DUuid(const DUuid& uuid)
{
  this->mUuid = new (std::nothrow) boost::uuids::uuid();
  *this->mUuid = *uuid.mUuid;
}

DUuid& DUuid::operator=(const DUuid& uuid)
{
  if (this == &uuid) { return *this; }

  (*this->mUuid) = *uuid.mUuid;
  return *this;
}

DUuid::~DUuid()
{
  delete this->mUuid;
  this->mUuid = nullptr;
}

bool DUuid::HasValue() const
{
  return this->mUuid->is_nil() == false;
}

std::string DUuid::ToString() const
{
  return boost::lexical_cast<std::string>(this->mUuid);
}

DUuid::iterator DUuid::begin() noexcept
{
  return this->mUuid->begin();
}

DUuid::iterator DUuid::end() noexcept
{
  return this->mUuid->end();
}

DUuid::const_iterator DUuid::begin() const noexcept
{
  return this->mUuid->begin();
}

DUuid::const_iterator DUuid::end() const noexcept
{
  return this->mUuid->end();
}

} /// ::dy::math namespace