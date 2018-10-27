#ifndef GUARD_DY_HELPER_CONSTANT_STRINGSETTINGFILE_H
#define GUARD_DY_HELPER_CONSTANT_STRINGSETTINGFILE_H
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

namespace dy
{

MDY_SET_IMMUTABLE_STRING(gTestPath, "./Project/Meta/TestScene.dydat");
MDY_SET_IMMUTABLE_STRING(gSettingPathName, "./Project/Meta/TestSetting.dydat");
MDY_SET_IMMUTABLE_STRING(gTestCanvasMetaInfo, "./Project/Meta/TestCanvasWidget0.dydat");
MDY_SET_IMMUTABLE_STRING(gScriptResourceMetaInfo, "./Project/Meta/ScriptResource.dydat");
MDY_SET_IMMUTABLE_STRING(gPrefabResourceMetaInfo, "./Project/Meta/PrefabResource.dydat");
MDY_SET_IMMUTABLE_STRING(gWidget0MetaInfo, "./Project/Meta/TestCanvasWidget00.dydat");

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_CONSTANT_STRINGSETTINGFILE_H