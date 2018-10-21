#ifndef GUARD_DY_ELEMENT_ABSTRACT_ADYNAMECOUNTERMAP_H
#define GUARD_DY_ELEMENT_ABSTRACT_ADYNAMECOUNTERMAP_H
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

namespace dy
{

///
/// @class ADyNameCounterMap
/// @brief
///
MDY_ABSTRACT ADyNameCounterMap
{
public:
  virtual ~ADyNameCounterMap() = 0;

  ///
  /// @brief  Create counted object name.
  /// @param  name Input object name
  /// @return Newly created counted name. If input name is new on this instance, just return name.
  ///
  std::string CreateObjectName(const std::string& name) noexcept;

private:
  /// name counter map
  std::unordered_map<std::string, TU32> mNameCounterMap;
};

} /// ::dy namespace

#endif /// GUARD_DY_ELEMENT_ABSTRACT_ADYNAMECOUNTERMAP_H